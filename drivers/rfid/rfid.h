#ifndef RFID_H_
#define RFID_H_
#include <avr/io.h>
#include "../wifi/wifi.h"
// MF522 MFRC522 error codes.
typedef enum{MI_OK, // Everything A-OK.
			MI_NOTAGERR,  // No tag error
			MI_ERR // General error
			} rfidError;
			
#define MAX_LEN 16        // Maximum length of an array.
#define RFID_RST_PIN PINB1
#define RFID_PORT PORTB
#define RFID_PORT_DDR DDRB					
// MF522 Command word
#define     MFRC522_IDLE          0x00      // NO action; Cancel the current command
#define     MFRC522_MEM           0x01      // Store 25 byte into the internal buffer.
#define     MFRC522_GENID         0x02      // Generates a 10 byte random ID number.
#define     MFRC522_CALCCRC       0x03      // CRC Calculate or selftest.
#define     MFRC522_TRANSMIT      0x04      // Transmit data
#define     MFRC522_NOCMDCH       0x07      // No command change.
#define     MFRC522_RECEIVE       0x08      // Receive Data
#define     MFRC522_TRANSCEIVE    0x0C      // Transmit and receive data,
#define     MFRC522_AUTHENT       0x0E      // Authentication Key
#define     MFRC522_SOFTRESET     0x0F      // Reset

// Mifare_One tag command word
#define     MF1_REQIDL            0x26      // find the antenna area does not enter hibernation
#define     MF1_REQALL            0x52      // find all the tags antenna area
#define     MF1_ANTICOLL          0x93      // anti-collision
#define     MF1_SELECTTAG         0x93      // election tag
#define     MF1_AUTHENT1A         0x60      // authentication key A
#define     MF1_AUTHENT1B         0x61      // authentication key B
#define     MF1_READ              0x30      // Read Block
#define     MF1_WRITE             0xA0      // write block
#define     MF1_DECREMENT         0xC0      // debit
#define     MF1_INCREMENT         0xC1      // recharge
#define     MF1_RESTORE           0xC2      // transfer block data to the buffer
#define     MF1_TRANSFER          0xB0      // save the data in the buffer
#define     MF1_HALT              0x50      // Sleep


//------------------ MFRC522 registers---------------
//Page 0:Command and Status
#define     Reserved00            0x00
#define     CommandReg            0x01
#define     CommIEnReg            0x02
#define     DivIEnReg             0x03
#define     CommIrqReg            0x04
#define     DivIrqReg             0x05
#define     ErrorReg              0x06
#define     Status1Reg            0x07
#define     Status2Reg            0x08
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     Reserved01            0x0F
//Page 1:Command
#define     Reserved10            0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     Reserved11            0x1A
#define     Reserved12            0x1B
#define     MifareReg             0x1C
#define     Reserved13            0x1D
#define     Reserved14            0x1E
#define     SerialSpeedReg        0x1F
//Page 2:CFG
#define     Reserved20            0x20
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     Reserved21            0x23
#define     ModWidthReg           0x24
#define     Reserved22            0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsPReg              0x28
#define     ModGsPReg             0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
//Page 3:TestRegister
#define     Reserved30            0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39
#define     TestDAC2Reg           0x3A
#define     TestADCReg            0x3B
#define     Reserved31            0x3C
#define     Reserved32            0x3D
#define     Reserved33            0x3E
#define     Reserved34            0x3F
//-----------------------------------------------

/*
	@brief   does the setup for rfid chip.                                              
*/
void initRfid();

/*
  @brief   set specific bits in a register.
  @param   reg   The address a register.
  @param   mask  The mask to update the register with.                                              
*/
void setRegBits(unsigned char addr,unsigned char mask);

/*
  @brief   clear specific bits in a register.
  @param   reg   The address a register.
  @param   mask  The mask to update the register with.                                              
*/
void clearRegBits(unsigned char addr,unsigned char mask);
/*
	@brief   Writes value to a register.
	@param   addr  The address a register.
	@param   val   The value to write to a register.                                                 
*/
void writeToReg(unsigned char addr , unsigned char val);

/*
	@brief   Read value from a register.
	@param   addr  The address a register.
	@return  The value of the register.                                                                
*/
unsigned char readFromReg(unsigned char addr);

/*
	@brief   apply soft reset to chip                                                               
*/
void resetRfid();

/*
	@brief   Run digital self test
	@return  True if passes , False otherwise                                                             
	
*/
boolean runSelfTest();

/*
	@brief   get the firmware version of the chip
	@return  the firmware version of the chip                                                              
*/
unsigned char getFirmwareVer();

/*
	@brief   send command to tag
	@param cmd command to be sent
	@param data data needed to complete command
	@param dLen data length
	@param result result returned by tag
	@param rLen length of valid bits in the resulting value
	@return  rfidError                                                              
*/
rfidError sendTagCommand(unsigned char cmd ,unsigned char *data,int dLen,unsigned char *result,int *rLen);

/*
	@brief   Checks to see if there is a tag in the vicinity.
	@param mode  The mode we are requsting in.
	@param data data needed to complete command
	@return  rfidError                                                              
*/
rfidError scanForTag(unsigned char mode ,unsigned char *data);

/*
	@brief   send halt command to current tag
	@return  rfidError                                                              
*/

rfidError haltTag();

/*
	@brief   send halt command to current tag
	@param   data    The data to calculate the value for.
	@param   len     The length of the data.
	@param   result  The result of the CRC calculation.                                                              
*/
void calculateCRC(uint8_t *data, int len, uint8_t *result);

/*
	@brief   Handles collisions that might occur if there are multiple tags available.
	@param  serial  The serial nb of the tag.
	@return  rfidError                                                              
*/
rfidError antiCollision(uint8_t *serial);

/*
	@brief   dump tag serial in string
	@param  serial  The serial nb of the tag.
	@param  len  The serial length
	@param  dst string to dump serial in it                                                        
*/
void dumpSerial(uint8_t*serial,uint8_t len ,unsigned char*dst);
#endif /* RFID_H_ */