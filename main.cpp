#include "mbed.h"

#include "CANopen.h"

extern "C" {
#include "CO_CAN.h"
}

#define TMR_TASK_INTERVAL   (1000)          /* Interval of tmrTask thread in microseconds */
#define INCREMENT_1MS(var)  (var++)         /* Increment 1ms variable in tmrTask */

/* Global variables and objects */
volatile uint16_t   CO_timer1ms = 0U;   /* variable increments each millisecond */



DigitalOut led1(LED1);


/* CAN interrupt function *****************************************************/
void CO_CAN1InterruptHandler_RX(void){
    CO_CANinterrupt_RX(CO->CANmodule[0]);
}

void CO_CAN1InterruptHandler_TX(void){
    CO_CANinterrupt_TX(CO->CANmodule[0]);
}



// main() runs in its own thread in the OS
int main() 
{
    CO_NMT_reset_cmd_t reset = CO_RESET_NOT;

    /* Configure microcontroller. */


    /* initialize EEPROM */


    /* increase variable each startup. Variable is stored in EEPROM. */
    OD_powerOnCounter++;

    while(reset != CO_RESET_APP)
    {
        /* CANopen communication reset - initialize CANopen objects *******************/
        CO_ReturnError_t err;
        uint16_t timer1msPrevious;

        /* disable CAN and CAN interrupts */


        /* initialize CANopen */
        err = CO_init(0/* CAN module address */, 10/* NodeID */, 125 /* bit rate */);
        if(err != CO_ERROR_NO){
            while(1);
            /* CO_errorReport(CO->em, CO_EM_MEMORY_ALLOCATION_ERROR, CO_EMC_SOFTWARE_INTERNAL, err); */
        }


        /* Configure Timer interrupt function for execution every 1 millisecond */


        /* Configure CAN transmit and receive interrupt */
        CANport->attach(&CO_CAN1InterruptHandler_RX, CAN::RxIrq);
        CANport->attach(&CO_CAN1InterruptHandler_TX, CAN::TxIrq);


        /* start CAN */
        CO_CANsetNormalMode(CO->CANmodule[0]);

        reset = CO_RESET_NOT;
        timer1msPrevious = CO_timer1ms;

        while(reset == CO_RESET_NOT)
        {
            /* loop for normal program execution ******************************************/
            uint16_t timer1msCopy, timer1msDiff;

            timer1msCopy = CO_timer1ms;
            timer1msDiff = timer1msCopy - timer1msPrevious;
            timer1msPrevious = timer1msCopy;


            /* CANopen process */
            reset = CO_process(CO, timer1msDiff, NULL);

            /* Nonblocking application code may go here. */

            /* Process EEPROM */
        }
    }


    /* program exit ***************************************************************/
    /* stop threads */


    /* delete objects from memory */
    CO_delete(0/* CAN module address */);


    /* reset */
    return 0;
}


/* timer thread executes in constant intervals ********************************/
static void tmrTask_thread(void)
{

    for(;;) {

        /* sleep for interval */

        INCREMENT_1MS(CO_timer1ms);

        if(CO->CANmodule[0]->CANnormal) {
            bool_t syncWas;

            /* Process Sync and read inputs */
            syncWas = CO_process_SYNC_RPDO(CO, TMR_TASK_INTERVAL);

            /* Further I/O or nonblocking application code may go here. */

            /* Write outputs */
            CO_process_TPDO(CO, syncWas, TMR_TASK_INTERVAL);

            /* verify timer overflow */
            if(0) {
                CO_errorReport(CO->em, CO_EM_ISR_TIMER_OVERFLOW, CO_EMC_SOFTWARE_INTERNAL, 0U);
            }
        }
    }
}