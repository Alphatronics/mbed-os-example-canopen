#ifndef CONFIG_H
#define CONFIG_H

#define CAN_MODULE_ADDRESS  CAN_1   // the hardware address of the can bus, since our STM32F091 has only one CAN bus this param is of no use so any value is OK
#define CANOPEN_NODE_ID     0x0A    // CANopen NODE ID
#define CAN_BITRATE         250     // in kbps

#define CANOPEN_DEVICE_TYPE 0x0L          //CANopen device type (0 = non-standard)
#define MANUFACTURER_DEVICENAME {'A', 'P', '1', '2', '3', '4'}
#define HARDWARE_VERSION        {'1', '.', '0', 'A'}
#define SOFTWARE_VERSION        {'1', '9', '.', '1', '.', '0' , '.', '0', '0', '0'}
#define VENDOR_ID           0x00000000L   // 32-bit vendor ID
#define PRODUCT_CODE        0x00000000L   // 32-bit product code
#define REVISION_NUMBER     0x00000000L   // 32-bit revision number

#if MBED_CONF_APP_DEVELOPER_MODE == 1
/**
 * The STM32 factory-programmed UUID memory.
 * Three values of 32 bits each starting at this address
 * Use like this: STM32_UUID[0], STM32_UUID[1], STM32_UUID[2]
 * (needed for developer fcc)
 */
#define STM32_UUID ((uint32_t *)0x1FFF7A10)
#define SERIAL_NUMBER       STM32_UUID[2]   // 32-bit serial number
#else
#define SERIAL_NUMBER       0x00000000L   // 32-bit serial number
#endif

#endif //CONFIG_H