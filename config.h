#ifndef CONFIG_H
#define CONFIG_H

#define CAN_MODULE_ADDRESS          CAN_1   // the hardware address of the can bus, since our STM32F091 has only one CAN bus this param is of no use so any value is OK
#define CANOPEN_DEFAULT_NODE_ID     0x0A    // CANopen NODE ID
#define CAN_BITRATE                 250     // in kbps

#endif //CONFIG_H