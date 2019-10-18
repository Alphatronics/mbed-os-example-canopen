/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#include "CANopen.h"
#include "config.h"

extern "C" {
#include "CO_driver.h"
//#include "CO_CAN.h"
}


volatile uint16_t   CO_timer1ms = 0U;        /* variable increments each millisecond */
Thread bgThread;
DigitalOut userLed(LED1, 1);
DigitalIn userButton(USER_BUTTON);


static void tmrTask_thread(void);



// main() runs in its own thread in the OS
int main() 
{
    printf("######## START ##########\n");
    printf("STATE MACHINE: boot\n");
    OD_powerOnCounter++;    // increment boot counter

    bgThread.start(callback(tmrTask_thread));

    CO_NMT_reset_cmd_t resetType = CO_RESET_NOT;
    while(resetType != CO_RESET_APP)
    {
        // initialize CANopen
        CO_ReturnError_t err = CO_init(CAN_MODULE_ADDRESS, CANOPEN_NODE_ID, CAN_BITRATE);
        if(err != CO_ERROR_NO){
            printf("ERROR: CO_init, code: %d\n", err);
            ThisThread::sleep_for(3000);   //sleep 3 seconds
            // CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err); 
            break;
        }

        // allow bgthread to process SYNC, RPDO and TPDO
        printf("Enabling bgthread processing\n");
        CO_CANsetNormalMode(CO->CANmodule[0]);

        resetType = CO_RESET_NOT;
        uint16_t previousTicksMs = CO_timer1ms;

        printf("STATE MACHINE: normal operation\n");
        while(resetType == CO_RESET_NOT)
        {
            // STATE MACHINE: loop for normal program execution 

            // calc the amount of millis have passed since we last ran this loop
            uint16_t currentTicksMs = CO_timer1ms;
            uint16_t millisDiff = currentTicksMs - previousTicksMs;
            previousTicksMs = currentTicksMs;

            // CANopen process (note: millisDiff will typically be 0ms or 1ms since this code loops faster than the bgThread)
            resetType = CO_process(CO, millisDiff, NULL);

            // Nonblocking application code may go here.
            // ex: killing the watchdog may be a good idea
            // ex: process EEPROM
        }

        printf("STATE MACHINE: communication reset\n");
        // STATE MACHINE: CANopen communication reset - cleanup and reinitialize CANopen objects 
        CO_CANreset();
    }


    // program exit: stop threads
    printf("Program exit!!\n");
    bgThread.terminate();

    // delete objects from memory
    CO_delete(CAN_MODULE_ADDRESS);

    // reset
    system_reset();
    return 0;
}


// timer thread executes in constant intervals
static void tmrTask_thread(void)
{
    static const int TMR_TASK_INTERVAL_1MS_INMS = 1;                               // Interval of tmrTask thread in millis
    static const int TMR_TASK_INTERVAL_1MS_INUS = TMR_TASK_INTERVAL_1MS_INMS*1000;       // Interval of tmrTask thread in micros
    for(;;) {
        ThisThread::sleep_for(TMR_TASK_INTERVAL_1MS_INMS);   //sleep 1 ms

        CO_timer1ms++;

        if(CO->CANmodule[0]->CANnormal) {
            bool_t syncWas;

            // Process Sync and read RPDO
            syncWas = CO_process_SYNC_RPDO(CO, TMR_TASK_INTERVAL_1MS_INUS);

            //apply RPDO value to DigitalOut(s)
            uint8_t outputmap = OD_writeOutput8Bit[0];
            userLed.write(outputmap & 0x01);                //userLed is first bit of RPDO-0 (200+nodeid)

            //apply DigitalIn(s) value to TPDO
            uint8_t inputmap = (userButton.read() == 0 ? 1 : 0) & 0x01;    //(inverted) userButton is first bit of TPDO-0 (180+nodeid)
            OD_readInput8Bit[0] = inputmap;

            OD_temperature[ODA_temperature_mainPCB] = 25;

            // Process TPDO
            CO_process_TPDO(CO, syncWas, TMR_TASK_INTERVAL_1MS_INUS);

            // verify timer overflow
            if(0) {
                CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0U);
            }
        }
    }
}