/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "main.h"

#include "mbed.h"

#include "CANopen.h"

extern "C" {
#include "CO_driver.h"
#include "CO_CAN.h"
}


volatile uint16_t   CO_timer1ms = 0U;        /* variable increments each millisecond */
Thread bgThread;
DigitalOut userLed(LED1);


#define CAN_MODULE_ADDRESS  CAN_1   // the hardware address of the can bus, since our STM32F091 has only one CAN bus this param is of no use so any value is OK
#define CANOPEN_NODE_ID     10      // CANopen NODE ID
#define CAN_BITRATE         250     // in kbps


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
        uint16_t timer1msPrevious;

        /* initialize CANopen */
        CO_ReturnError_t err = CO_init(CAN_MODULE_ADDRESS, CANOPEN_NODE_ID, CAN_BITRATE);
        if(err != CO_ERROR_NO){
            printf("ERROR: CO_init, code: %d\n", err);
            ThisThread::sleep_for(3000);   //sleep 3 seconds
            /* CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err); */
            break;
        }

        /* Configure CAN transmit and receive interrupt */
        CANport->attach(&CO_CAN_RX_InterruptHandler, CAN::RxIrq);
        CANport->attach(&CO_CAN_TX_InterruptHandler, CAN::TxIrq);


        /* start CAN */
        printf("Start CAN\n");
        CO_CANsetNormalMode(CO->CANmodule[0]);

        resetType = CO_RESET_NOT;
        timer1msPrevious = CO_timer1ms;

        printf("STATE MACHINE: normal operation\n");
        while(resetType == CO_RESET_NOT)
        {
            /* STATE MACHINE: loop for normal program execution ******************************************/
            uint16_t timer1msCopy, timer1msDiff;

            timer1msCopy = CO_timer1ms;
            timer1msDiff = timer1msCopy - timer1msPrevious;
            timer1msPrevious = timer1msCopy;


            /* CANopen process */
            resetType = CO_process(CO, timer1msDiff, NULL);

            /* Nonblocking application code may go here. */

            /* Process EEPROM */
        }

        printf("STATE MACHINE: communication reset\n");
        /* STATE MACHINE: CANopen communication reset - cleanup and reinitialize CANopen objects *******************/
        CO_CANreset();
    }


    /* program exit ***************************************************************/
    /* stop threads */
    printf("Program exit!!\n");
    bgThread.terminate();

    /* delete objects from memory */
    CO_delete(0/* CAN module address */);

    /* reset */
    system_reset();
    return 0;
}


/* timer thread executes in constant intervals ********************************/
static void tmrTask_thread(void)
{
    static const int TMR_TASK_INTERVAL_MS = 1;                               // Interval of tmrTask thread in millis
    static const int TMR_TASK_INTERVAL_US = TMR_TASK_INTERVAL_MS*1000;       // Interval of tmrTask thread in micros
    for(;;) {
        ThisThread::sleep_for(TMR_TASK_INTERVAL_MS);   //sleep 1 ms

        CO_timer1ms++;

        if(CO->CANmodule[0]->CANnormal) {
            bool_t syncWas;

            /* Process Sync and read inputs */
            syncWas = CO_process_SYNC_RPDO(CO, TMR_TASK_INTERVAL_US);

            /* Further I/O or nonblocking application code may go here. */


            /* Write outputs */
            CO_process_TPDO(CO, syncWas, TMR_TASK_INTERVAL_US);

            /* verify timer overflow */
            if(0) {
                CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0U);
            }

            if(CO_timer1ms % 50 == 0) {
                userLed = !userLed;
            }
        }
    }
}


/* CAN interrupt function *****************************************************/
void CO_CAN_RX_InterruptHandler(void) {
    CO_CANinterrupt_RX(CO->CANmodule[0]);
}

void CO_CAN_TX_InterruptHandler(void) {
    CO_CANinterrupt_TX(CO->CANmodule[0]);
}