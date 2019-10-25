/*
 * CANopen Object Dictionary.
 *
 * This file was automatically generated with CANopenNode Object
 * Dictionary Editor. DON'T EDIT THIS FILE MANUALLY !!!!
 * Object Dictionary Editor is currently an older, but functional web
 * application. For more info see See 'Object_Dictionary_Editor/about.html' in
 * <http://sourceforge.net/p/canopennode/code_complete/ci/master/tree/>
 * For more information on CANopen Object Dictionary see <CO_SDO.h>.
 *
 * @file        CO_OD.h
 * @author      Janez Paternoster
 * @copyright   2010 - 2016 Janez Paternoster
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <https://github.com/CANopenNode/CANopenNode>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * CANopenNode is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Following clarification and special exception to the GNU General Public
 * License is included to the distribution terms of CANopenNode:
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library. Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 *
 * As a special exception, the copyright holders of this library give
 * you permission to link this library with independent modules to
 * produce an executable, regardless of the license terms of these
 * independent modules, and to copy and distribute the resulting
 * executable under terms of your choice, provided that you also meet,
 * for each linked independent module, the terms and conditions of the
 * license of that module. An independent module is a module which is
 * not derived from or based on this library. If you modify this
 * library, you may extend this exception to your version of the
 * library, but you are not obliged to do so. If you do not wish
 * to do so, delete this exception statement from your version.
 */


#ifndef CO_OD_H
#define CO_OD_H

#include "config.h"


/*******************************************************************************
   FILE INFO:
      FileName:     IO Example
      FileVersion:  -
      CreationTime: 13:04:00
      CreationDate: 2019-10-10
      CreatedBy:    GVL
*******************************************************************************/


/*******************************************************************************
   DEVICE INFO:
      VendorName:     Alphatronics
      VendorNumber:   0
      ProductName:    AP1234
      ProductNumber:  0
*******************************************************************************/


/*******************************************************************************
   FEATURES
*******************************************************************************/
   #define CO_NO_SYNC                     1   //Associated objects: 1005, 1006, 1007, 2103, 2104
   #define CO_NO_EMERGENCY                1   //Associated objects: 1014, 1015
   #define CO_NO_SDO_SERVER               1   //Associated objects: 1200, 1F50
   #define CO_NO_SDO_CLIENT               0   
   #define CO_NO_RPDO                     4   //Associated objects: 1400, 1401, 1402, 1403, 1600, 1601, 1602, 1603
   #define CO_NO_TPDO                     4   //Associated objects: 1800, 1801, 1802, 1803, 1A00, 1A01, 1A02, 1A03
   #define CO_NO_NMT_MASTER               0   
   #define CO_NO_TRACE                    0   
   #define CO_NO_LSS_SERVER               0
   #define CO_NO_LSS_CLIENT               0

/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             56


/*******************************************************************************
   TYPE DEFINITIONS FOR RECORDS
*******************************************************************************/
/*1018      */ typedef struct{
               uint8_t      maxSubIndex;
               uint32_t     vendorID;
               uint32_t     productCode;
               uint32_t     revisionNumber;
               uint32_t     serialNumber;
               }              OD_identity_t;

/*1200[1]   */ typedef struct{
               uint8_t      maxSubIndex;
               uint32_t     COB_IDClientToServer;
               uint32_t     COB_IDServerToClient;
               }              OD_SDOServerParameter_t;

/*1400[4]   */ typedef struct{
               uint8_t      maxSubIndex;
               uint32_t     COB_IDUsedByRPDO;
               uint8_t      transmissionType;
               }              OD_RPDOCommunicationParameter_t;

/*1600[4]   */ typedef struct{
               uint8_t      numberOfMappedObjects;
               uint32_t     mappedObject1;
               uint32_t     mappedObject2;
               uint32_t     mappedObject3;
               uint32_t     mappedObject4;
               uint32_t     mappedObject5;
               uint32_t     mappedObject6;
               uint32_t     mappedObject7;
               uint32_t     mappedObject8;
               }              OD_RPDOMappingParameter_t;

/*1800[4]   */ typedef struct{
               uint8_t      maxSubIndex;
               uint32_t     COB_IDUsedByTPDO;
               uint8_t      transmissionType;
               uint16_t     inhibitTime;
               uint8_t      compatibilityEntry;
               uint16_t     eventTimer;
               uint8_t      SYNCStartValue;
               }              OD_TPDOCommunicationParameter_t;

/*1A00[4]   */ typedef struct{
               uint8_t      numberOfMappedObjects;
               uint32_t     mappedObject1;
               uint32_t     mappedObject2;
               uint32_t     mappedObject3;
               uint32_t     mappedObject4;
               uint32_t     mappedObject5;
               uint32_t     mappedObject6;
               uint32_t     mappedObject7;
               uint32_t     mappedObject8;
               }              OD_TPDOMappingParameter_t;

/*1F50[1]   */ typedef struct{
               uint8_t      maxSubIndex;
               domain_t         domain_t;
               }              OD_ProgramDownload_t;

/*2120      */ typedef struct{
               uint8_t      maxSubIndex;
               int64_t      I64;
               uint64_t     U64;
               float32_t         R32;
               float64_t         R64;
               domain_t         domain_t;
               }              OD_testVar_t;

/*2130      */ typedef struct{
               uint8_t      maxSubIndex;
               char_t string[30];
               uint64_t     epochTimeBaseMs;
               uint32_t     epochTimeOffsetMs;
               }              OD_time_t;


/*******************************************************************************
   STRUCTURES FOR VARIABLES IN DIFFERENT MEMORY LOCATIONS
*******************************************************************************/
#define  CO_OD_FIRST_LAST_WORD     0x55 //Any value from 0x01 to 0xFE. If changed, EEPROM will be reinitialized.

/***** Structure for RAM variables ********************************************/
struct sCO_OD_RAM{
               uint32_t     FirstWord;

/*1001      */ uint8_t      errorRegister;
/*1002      */ uint32_t     manufacturerStatusRegister;
/*1003      */ uint32_t     preDefinedErrorField[8];
/*1010      */ uint32_t     storeParameters[1];
/*1011      */ uint32_t     restoreDefaultParameters[1];
/*2100      */ oChar_t   errorStatusBits[10];
/*2103      */ uint16_t     SYNCCounter;
/*2104      */ uint16_t     SYNCTime;
/*2107      */ uint16_t     performance[5];
/*2108      */ int16_t      temperature[1];
/*2109      */ int16_t      voltage[1];
/*2110      */ int32_t      variableInt32[16];
/*2120      */ OD_testVar_t   testVar;
/*2130      */ OD_time_t      time;
/*6000      */ uint8_t      readInput8Bit[8];
/*6200      */ uint8_t      writeOutput8Bit[8];
/*6401      */ int16_t      readAnalogueInput16Bit[12];
/*6411      */ int16_t      writeAnalogueOutput16Bit[8];

               uint32_t     LastWord;
};

/***** Structure for EEPROM variables *****************************************/
struct sCO_OD_EEPROM{
               uint32_t     FirstWord;

/*2106      */ uint32_t     powerOnCounter;
/*2112      */ int32_t      variableNVInt32[16];

               uint32_t     LastWord;
};


/***** Structure for ROM variables ********************************************/
struct sCO_OD_ROM{
               uint32_t     FirstWord;

/*1000      */ uint32_t     deviceType;
/*1005      */ uint32_t     COB_ID_SYNCMessage;
/*1006      */ uint32_t     communicationCyclePeriod;
/*1007      */ uint32_t     synchronousWindowLength;
/*1008      */ char_t manufacturerDeviceName[6];
/*1009      */ char_t manufacturerHardwareVersion[4];
/*100A      */ char_t manufacturerSoftwareVersion[10];
/*1014      */ uint32_t     COB_ID_EMCY;
/*1015      */ uint16_t     inhibitTimeEMCY;
/*1016      */ uint32_t     consumerHeartbeatTime[4];
/*1017      */ uint16_t     producerHeartbeatTime;
/*1018      */ OD_identity_t  identity;
/*1019      */ uint8_t      synchronousCounterOverflowValue;
/*1029      */ uint8_t      errorBehavior[6];
/*1200[1]   */ OD_SDOServerParameter_t SDOServerParameter[1];
/*1400[4]   */ OD_RPDOCommunicationParameter_t RPDOCommunicationParameter[4];
/*1600[4]   */ OD_RPDOMappingParameter_t RPDOMappingParameter[4];
/*1800[4]   */ OD_TPDOCommunicationParameter_t TPDOCommunicationParameter[4];
/*1A00[4]   */ OD_TPDOMappingParameter_t TPDOMappingParameter[4];
/*1F50      */ OD_ProgramDownload_t ProgramDownload;
/*1F80      */ uint32_t     NMTStartup;
/*2101      */ uint8_t      CANNodeID;
/*2102      */ uint16_t     CANBitRate;
/*2111      */ int32_t      variableROMInt32[16];

               uint32_t     LastWord;
};


/***** Declaration of Object Dictionary variables *****************************/
extern struct sCO_OD_RAM CO_OD_RAM;

extern struct sCO_OD_EEPROM CO_OD_EEPROM;

extern struct sCO_OD_ROM CO_OD_ROM;


/*******************************************************************************
   ALIASES FOR OBJECT DICTIONARY VARIABLES
*******************************************************************************/
/*1000, Data Type: uint32_t */
      #define OD_deviceType                              CO_OD_ROM.deviceType

/*1001, Data Type: uint8_t */
      #define OD_errorRegister                           CO_OD_RAM.errorRegister

/*1002, Data Type: uint32_t */
      #define OD_manufacturerStatusRegister              CO_OD_RAM.manufacturerStatusRegister

/*1003, Data Type: uint32_t, Array[8] */
      #define OD_preDefinedErrorField                    CO_OD_RAM.preDefinedErrorField
      #define ODL_preDefinedErrorField_arrayLength       8

/*1005, Data Type: uint32_t */
      #define OD_COB_ID_SYNCMessage                      CO_OD_ROM.COB_ID_SYNCMessage

/*1006, Data Type: uint32_t */
      #define OD_communicationCyclePeriod                CO_OD_ROM.communicationCyclePeriod

/*1007, Data Type: uint32_t */
      #define OD_synchronousWindowLength                 CO_OD_ROM.synchronousWindowLength

/*1008, Data Type: char_t, Array[11] */
      #define OD_manufacturerDeviceName                  CO_OD_ROM.manufacturerDeviceName
      #define ODL_manufacturerDeviceName_stringLength    6

/*1009, Data Type: char_t, Array[4] */
      #define OD_manufacturerHardwareVersion             CO_OD_ROM.manufacturerHardwareVersion
      #define ODL_manufacturerHardwareVersion_stringLength 4

/*100A, Data Type: char_t, Array[4] */
      #define OD_manufacturerSoftwareVersion             CO_OD_ROM.manufacturerSoftwareVersion
      #define ODL_manufacturerSoftwareVersion_stringLength 10

/*1010, Data Type: uint32_t, Array[1] */
      #define OD_storeParameters                         CO_OD_RAM.storeParameters
      #define ODL_storeParameters_arrayLength            1
      #define ODA_storeParameters_saveAllParameters      0

/*1011, Data Type: uint32_t, Array[1] */
      #define OD_restoreDefaultParameters                CO_OD_RAM.restoreDefaultParameters
      #define ODL_restoreDefaultParameters_arrayLength   1
      #define ODA_restoreDefaultParameters_restoreAllDefaultParameters 0

/*1014, Data Type: uint32_t */
      #define OD_COB_ID_EMCY                             CO_OD_ROM.COB_ID_EMCY

/*1015, Data Type: uint16_t */
      #define OD_inhibitTimeEMCY                         CO_OD_ROM.inhibitTimeEMCY

/*1016, Data Type: uint32_t, Array[4] */
      #define OD_consumerHeartbeatTime                   CO_OD_ROM.consumerHeartbeatTime
      #define ODL_consumerHeartbeatTime_arrayLength      4

/*1017, Data Type: uint16_t */
      #define OD_producerHeartbeatTime                   CO_OD_ROM.producerHeartbeatTime

/*1018, Data Type: OD_identity_t */
      #define OD_identity                                CO_OD_ROM.identity

/*1019, Data Type: uint8_t */
      #define OD_synchronousCounterOverflowValue         CO_OD_ROM.synchronousCounterOverflowValue

/*1029, Data Type: uint8_t, Array[6] */
      #define OD_errorBehavior                           CO_OD_ROM.errorBehavior
      #define ODL_errorBehavior_arrayLength              6
      #define ODA_errorBehavior_communication            0
      #define ODA_errorBehavior_communicationOther       1
      #define ODA_errorBehavior_communicationPassive     2
      #define ODA_errorBehavior_generic                  3
      #define ODA_errorBehavior_deviceProfile            4
      #define ODA_errorBehavior_manufacturerSpecific     5

/*1200[1], Data Type: OD_SDOServerParameter_t, Array[1] */
      #define OD_SDOServerParameter                      CO_OD_ROM.SDOServerParameter

/*1400[4], Data Type: OD_RPDOCommunicationParameter_t, Array[4] */
      #define OD_RPDOCommunicationParameter              CO_OD_ROM.RPDOCommunicationParameter

/*1600[4], Data Type: OD_RPDOMappingParameter_t, Array[4] */
      #define OD_RPDOMappingParameter                    CO_OD_ROM.RPDOMappingParameter

/*1800[4], Data Type: OD_TPDOCommunicationParameter_t, Array[4] */
      #define OD_TPDOCommunicationParameter              CO_OD_ROM.TPDOCommunicationParameter

/*1A00[4], Data Type: OD_TPDOMappingParameter_t, Array[4] */
      #define OD_TPDOMappingParameter                    CO_OD_ROM.TPDOMappingParameter

/*1F80, Data Type: uint32_t */
      #define OD_NMTStartup                              CO_OD_ROM.NMTStartup

/*2100, Data Type: oChar_t, Array[10] */
      #define OD_errorStatusBits                         CO_OD_RAM.errorStatusBits
      #define ODL_errorStatusBits_stringLength           10

/*2101, Data Type: uint8_t */
      #define OD_CANNodeID                               CO_OD_ROM.CANNodeID

/*2102, Data Type: uint16_t */
      #define OD_CANBitRate                              CO_OD_ROM.CANBitRate

/*2103, Data Type: uint16_t */
      #define OD_SYNCCounter                             CO_OD_RAM.SYNCCounter

/*2104, Data Type: uint16_t */
      #define OD_SYNCTime                                CO_OD_RAM.SYNCTime

/*2106, Data Type: uint32_t */
      #define OD_powerOnCounter                          CO_OD_EEPROM.powerOnCounter

/*2107, Data Type: uint16_t, Array[5] */
      #define OD_performance                             CO_OD_RAM.performance
      #define ODL_performance_arrayLength                5
      #define ODA_performance_cyclesPerSecond            0
      #define ODA_performance_timerCycleTime             1
      #define ODA_performance_timerCycleMaxTime          2
      #define ODA_performance_mainCycleTime              3
      #define ODA_performance_mainCycleMaxTime           4

/*2108, Data Type: int16_t, Array[1] */
      #define OD_temperature                             CO_OD_RAM.temperature
      #define ODL_temperature_arrayLength                1
      #define ODA_temperature_mainPCB                    0

/*2109, Data Type: int16_t, Array[1] */
      #define OD_voltage                                 CO_OD_RAM.voltage
      #define ODL_voltage_arrayLength                    1
      #define ODA_voltage_mainPCBSupply                  0

/*2110, Data Type: int32_t, Array[16] */
      #define OD_variableInt32                           CO_OD_RAM.variableInt32
      #define ODL_variableInt32_arrayLength              16

/*2111, Data Type: int32_t, Array[16] */
      #define OD_variableROMInt32                        CO_OD_ROM.variableROMInt32
      #define ODL_variableROMInt32_arrayLength           16

/*2112, Data Type: int32_t, Array[16] */
      #define OD_variableNVInt32                         CO_OD_EEPROM.variableNVInt32
      #define ODL_variableNVInt32_arrayLength            16

/*2120, Data Type: OD_testVar_t */
      #define OD_testVar                                 CO_OD_RAM.testVar

/*2130, Data Type: OD_time_t */
      #define OD_time                                    CO_OD_RAM.time

/*6000, Data Type: uint8_t, Array[8] */
      #define OD_readInput8Bit                           CO_OD_RAM.readInput8Bit
      #define ODL_readInput8Bit_arrayLength              8

/*6200, Data Type: uint8_t, Array[8] */
      #define OD_writeOutput8Bit                         CO_OD_RAM.writeOutput8Bit
      #define ODL_writeOutput8Bit_arrayLength            8

/*6401, Data Type: int16_t, Array[12] */
      #define OD_readAnalogueInput16Bit                  CO_OD_RAM.readAnalogueInput16Bit
      #define ODL_readAnalogueInput16Bit_arrayLength     12

/*6411, Data Type: int16_t, Array[8] */
      #define OD_writeAnalogueOutput16Bit                CO_OD_RAM.writeAnalogueOutput16Bit
      #define ODL_writeAnalogueOutput16Bit_arrayLength   8


#endif

