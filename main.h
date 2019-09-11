#ifndef MAIN_H_
#define MAIN_H_

int main(void);

static void tmrTask_thread(void);

void CO_CAN_RX_InterruptHandler(void);
void CO_CAN_TX_InterruptHandler(void);

#endif // MAIN_H_