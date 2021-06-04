/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "CANopen.h"
#include "config.h"

extern "C" {
#include "CO_driver.h"
}
#include "CO_Indicators.h"


// flag used to stop threads
#define STOP_THREADS_FLAG   1
// Interval of tmrTask thread in micros: should always be 1ms!
static const int RTTHREAD_INTERVAL_1000US = 1000;
// variable increments each millisecond
volatile uint16_t CO_timer1ms = 0U;
// thread object that holds the RT thread
Thread* rtThread;
// function executed by RT thread
static void rtTask(void);
// Timer used to measure performance of RT thread
Timer rtPerfTimer;



DigitalOut userLed(LED2, 1);
DigitalIn userButton(USER_BUTTON);




// main() runs in its own thread in the OS
int main() 
{
    printf("######## CANOPEN DEMO (" __DATE__ ", " __TIME__ ") ##########\n");
    OD_powerOnCounter++;    // increment boot counter

    CO_NMT_reset_cmd_t resetCmd = CO_RESET_NOT;
    while(resetCmd != CO_RESET_APP && resetCmd != CO_RESET_QUIT)
    {
        printf("####### start comms-loop #######\n");

        // initialize CANopen
        CO_ReturnError_t err = CO_init((void*)CAN_MODULE_ADDRESS, CANOPEN_DEFAULT_NODE_ID, CAN_BITRATE);
        if(err != CO_ERROR_NO){
            printf("ERROR: CO_init, code: %d\n", err);
            ThisThread::sleep_for(3000);   // in ms
            break;
        }

        // allow rtThread to process SYNC, RPDO and TPDO
        printf("Enabling rtThread processing\n");
        rtThread = new Thread(osPriorityAboveNormal, 8 * 1024, NULL, "rtThread");
        osStatus threadstate = rtThread->start(callback(rtTask));
        if(threadstate != osOK) {
            printf("Failed starting rtThread\n");
        }

        CO_CANsetNormalMode(CO->CANmodule[0]);

        resetCmd = CO_RESET_NOT;
        uint16_t previousTicksMs = CO_timer1ms;

        printf("CANopen ready\n");
        while(resetCmd == CO_RESET_NOT)
        {
            // calc the amount of millis have passed since we last ran this loop
            uint16_t currentTicksMs = CO_timer1ms;
            uint16_t millisDiff = currentTicksMs - previousTicksMs;
            previousTicksMs = currentTicksMs;

            // CANopen process (note: millisDiff will typically be 0ms or 1ms since this code loops faster than the rtThread)
            resetCmd = CO_process(CO, millisDiff, NULL);

            // Nonblocking application code may go here.
            // ex: killing the watchdog may be a good idea
            // ex: process EEPROM
        }

        printf("CANopen communication reset\n");
        rtThread->flags_set(STOP_THREADS_FLAG);
        rtThread->join();
        delete(rtThread);
        rtThread = NULL;

        // delete objects from memory
        CO_delete((void*)CAN_MODULE_ADDRESS); // => may crash application if CO_init failed

        // STATE MACHINE: CANopen communication reset - cleanup and reinitialize CANopen objects 
        CO_CANreset();
    }

    // program exit: stop threads
    printf("Program exit!!\n");

    // reset
    if(resetCmd != CO_RESET_QUIT)
        system_reset();
    return 0;
}


static void rtLoop(void)
{
    // Process Sync and read RPDO
    bool_t syncWas = CO_process_SYNC(CO, RTTHREAD_INTERVAL_1000US);
    CO_process_RPDO(CO, syncWas);

    //apply RPDO value to DigitalOut(s)
    // note: userLed is first bit of RPDO-0
    uint8_t outputmap = OD_writeOutput8Bit[0];
    userLed.write(outputmap & 0x01);

    //apply DigitalIn(s) value to TPDO
    // note: userButton is first bit of TPDO-0 and is also inverted
    uint8_t inputmap = (userButton.read() == 0 ? 1 : 0) & 0x01;
    OD_readInput8Bit[0] = inputmap;

    // Process TPDO
    CO_process_TPDO(CO, syncWas, RTTHREAD_INTERVAL_1000US);

    //Update LED indicators
    CO_Indicators_process(CO->NMT);
}


// timer thread executes in constant intervals
static void rtTask(void)
{
    rtPerfTimer.start();

    // Sleep for exact 1ms.
    // It schedules this this thread inactive which allows the main thread
    // to continue execution. It would be better if we could sleep more precisely
    // using nanos instead, however mbed doesn't allow that. The benefit would
    // be that we could sleep only the time needed to fit exactly 
    // 1 ms taking into account the time to execute the RT code below.
    while(!ThisThread::flags_wait_any_for(STOP_THREADS_FLAG, RTTHREAD_INTERVAL_1000US/1000)) {

        CO_timer1ms++;

        if(CO->CANmodule[0]->CANnormal) {
            // We use a Timer to measure the RT thread execution time.
            // In case of bad performance, an EMCY msg will be created later on.
            rtPerfTimer.reset();
            int begin = rtPerfTimer.read_us();

            // perform RT tasks
            // NOTE: DO NOT PERFORM ANY PRINTF ACTIONS IN THE RT_LOOP,
            // it's too slow and will trigger the bad performance timer
            // and hence put the device in pre-op mode until reset.
            rtLoop();

            //calculate time spend executing RT code, send EMCY in case of bad performance (>1ms)
            int timespan = rtPerfTimer.read_us() - begin;
            if(timespan > RTTHREAD_INTERVAL_1000US) {
                float tsMillis = static_cast<double>(timespan) / 1000;
                printf("RT slow: %.3fms\n", tsMillis);
                CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0U); 
                // => device will enter pre-op and must be reset
            }
        }
    }
}