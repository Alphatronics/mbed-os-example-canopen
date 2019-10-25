#include "CO_fwupgrade.h"
#include "inttypes.h" //for printing PRIx
#include "mbed.h"


//fix buffer to match page size of AT45DB32 NAND to keep memory usage low
#define PROG_BUFFER_SIZE        512
#define NAND_PAGES_PER_BLOCK    8
#define NAND_ERASE_SIZE         PROG_BUFFER_SIZE * NAND_PAGES_PER_BLOCK
static uint8_t _progbuff[PROG_BUFFER_SIZE];
static uint16_t _bytesInProgBuff;
static uint16_t _pagesStoredInNand;
static uint32_t _bytesTotalStoredInNand;
static bool _fwUpgradeCompleted = false;

extern BlockDevice* nand;


/**
 * Function that write the _progbuff content into AT45DB NAND.
 * 
 * Note that the AT45DB32 has a configurable page size, which can
 * either be set to 512 bytes or 528 bytes.
 * We default to 512 page size in our firmware, see "dataflash.binary-size": true.
 * With that, the device has following layout:
 * Page size: 512 bytes
 * Block size: 512 * 8 = 4kb (= 4096 bytes)
 * Sector size: 16 * 4kb = 64kb (= 65536 bytes)
 * Total size: 64kb * 64 = 4Mb (= 4096kb) (= 4.194.304 bytes) (= 32Mbit) 
 * Erase size: 4096 bytes (= block size -> mbed-os erases the NAND this way)
 * 
 * Function performs erase-program cycle, and syncs (blocking) this
 * action with the block device.
 * 
 */ 
void writeToStorage();
void writeToStorage()
{
    bd_addr_t address = _pagesStoredInNand * PROG_BUFFER_SIZE;

    //mbed erases per blocks, so only call erase per 8 pages
    if(_pagesStoredInNand % NAND_PAGES_PER_BLOCK == 0) {
        printf("erase-NAND: offset=%" PRIu64 "\n", address);
        nand->erase(address, NAND_ERASE_SIZE);
    }

    //note that we adjust program size so that trailing bytes are zero
    printf("program-NAND: offset=%" PRIu64 " length=%" PRIu16 "\n", address, _bytesInProgBuff);
    nand->program(_progbuff, address, PROG_BUFFER_SIZE);
    _bytesTotalStoredInNand += _bytesInProgBuff; 
    _pagesStoredInNand++;
    _bytesInProgBuff = 0;

    //always clear the buffer so that writing to NAND uses empty trailing bytes
    memset(_progbuff, 0, PROG_BUFFER_SIZE);
}

/*
 * Function for accessing _SDO program download (index 1f50).
 * 
 * When master sends more bytes to this object than defined in
 * CO_SDO_BUFFER_SIZE, multiple segments are received by this node.
 * For example, sending 38 bytes results in the following segments:
 * RX segment 1: datalength=28, first=true, last=false, offset=0, dataLengthTotal=38
 * RX segment 2: datalength=10, first=false, last=true, offset=28, dataLengthTotal=38

 * For more information see file CO_SDO.h.
 */
static CO_SDO_abortCode_t CO_ODF_1F50(CO_ODF_arg_t *ODF_arg);
static CO_SDO_abortCode_t CO_ODF_1F50(CO_ODF_arg_t *ODF_arg){
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    // allow on subindex 1 only
    if(ODF_arg->subIndex != 0x01)
        return ret;
    //only dowloading is supported
    if(ODF_arg->reading)
        return ret;

    if(ODF_arg->firstSegment) {
        //download started
        printf("FW Download started\n");
        _bytesInProgBuff = 0;
        _pagesStoredInNand = 0;
        _fwUpgradeCompleted = false;
        _bytesTotalStoredInNand=0;
        memset(_progbuff, 0, PROG_BUFFER_SIZE);
    }

    uint8_t percent = ( ODF_arg->offset * 100 / ODF_arg->dataLengthTotal);
    printf("downloaded data %d%% (datalength=%" PRIu16 ", first=%s, last=%s, offset=%" PRIu32 ", dataLengthTotal=%" PRIu32 ") \n", 
        percent, ODF_arg->dataLength, (ODF_arg->firstSegment ? "true":"false"), (ODF_arg->lastSegment ? "true":"false"), ODF_arg->offset, ODF_arg->dataLengthTotal);

    //check if we'll be overflowing the program buffer
    uint16_t bytesFromSegmentParsed = 0;
    uint16_t bytesFreeInProgBuf = PROG_BUFFER_SIZE - _bytesInProgBuff;
    if(ODF_arg->dataLength > bytesFreeInProgBuf) {
        //copy some segment data until buffer is full
        for(int i=0; i<bytesFreeInProgBuf; i++) {
            _progbuff[_bytesInProgBuff] = ODF_arg->data[i];
            _bytesInProgBuff++;
        }
        writeToStorage();
        bytesFromSegmentParsed += bytesFreeInProgBuf;
        bytesFreeInProgBuf = PROG_BUFFER_SIZE;
    }

    //copy remaining bytes from segment into buffer
    for(int i=bytesFromSegmentParsed; i<ODF_arg->dataLength; i++) {
        _progbuff[_bytesInProgBuff] = ODF_arg->data[i];
        _bytesInProgBuff++;
    }

    if(ODF_arg->lastSegment) {
        //download finished
        writeToStorage();
        printf("FW Download finished: %" PRIu32 " bytes stored\n", _bytesTotalStoredInNand);
        _fwUpgradeCompleted = true;
    }

    return ret;
}

void CO_FwUpgradeRegisterODFunctions(CO_t* CO)
{
    CO_OD_configure(*(CO->SDO), OD_H1F50_PRGRMDWNLOAD_OBJECT, CO_ODF_1F50, (void*)0U, 0U, 0U);
}


bool CO_FwUpgradeCompleted()
{
    return _fwUpgradeCompleted;
}

void CO_VerifyFwUpgradeCandidate()
{
    printf("FW Download verification...\n");
    int32_t bytesToRead = (int32_t)_bytesTotalStoredInNand;
    uint16_t pagesRed = 0;
    memset(_progbuff, 0, PROG_BUFFER_SIZE);

    while(bytesToRead > 0) {
        bd_addr_t address = pagesRed * PROG_BUFFER_SIZE;

        //read from NAND to buffer (including trailing bytes)
        nand->read(_progbuff, address, PROG_BUFFER_SIZE);
        pagesRed++;
        bytesToRead -= PROG_BUFFER_SIZE;
        printf("%s", (char *)_progbuff);
    }
    printf("\n");
}