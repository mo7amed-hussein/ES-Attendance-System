#include "rfid.h"
#include "../spi/spi.h"
#include <util/delay.h>
rfidError _rfidError = MI_OK ;
unsigned char tmp[50];
void initRfid()
{
	//init spi as master
	initSpiMaster();
	
	SPI_PORT |= (1<<SPI_SS_PIN);
	
	//set reset high
	RFID_PORT_DDR |= (1<<RFID_RST_PIN);
	RFID_PORT |= (1<<RFID_RST_PIN);
	
	_delay_ms(100);
	
	SPI_PORT |= (1<<SPI_SS_PIN);
	//reset chip
	resetRfid();
	//define settings for internal timer
	//Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
	writeToReg(TModeReg,0x8D); // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
	writeToReg(TPrescalerReg,0x3E);//// TModeReg[3..0] + TPrescalerReg
	
	//Defines the 16-bit timer reload value
	//reload value bits are contained in to 8-bit reg H,L
	writeToReg(TReloadRegL,30);
	writeToReg(TReloadRegH,0);
	//force 100 ASK transmit modulation
	writeToReg(TxAutoReg,0x40);
	//CRC initial value to 3636h
	//Defines general mode settings for transmitting and receiving
	writeToReg(ModeReg,0x3D);
	//turn antenna on
	setRegBits(TxControlReg,0x03);
}
void writeToReg(unsigned char addr , unsigned char val)
{
	//format address to be 0XXXXXX0
	//send register address
	SPI_PORT &= ~(1<<SPI_SS_PIN);
	tranceiveSpi((addr<<1)&0x7E);
	//send value
	tranceiveSpi(val);
	SPI_PORT |= (1<<SPI_SS_PIN);
}

unsigned char readFromReg(unsigned char addr)
{
	unsigned char val;
	//format address to be 0XXXXXX0
	//send register address , or with 0x80 for read
	SPI_PORT &= ~(1<<SPI_SS_PIN);
	val = tranceiveSpi(((addr<<1)&0x7E) | 0x80);
	//get value
	val=tranceiveSpi(0x00);
	SPI_PORT |= (1<<SPI_SS_PIN);
	return val;
}

void resetRfid()
{
	//send soft reset command to chip
	writeToReg(CommandReg,MFRC522_SOFTRESET);
}

unsigned char getFirmwareVer()
{
	unsigned char ver;
	//read version register
	ver = readFromReg(VersionReg);
	return ver;
}

boolean runSelfTest()
{
	unsigned char crcInt;//to store CRCIRq 
	unsigned int i ;//loop counter
	
	unsigned char selfTestResultV1[] = {0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
		0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
		0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
		0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
		0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
		0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
		0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
	0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79};
	
	unsigned char selfTestResultV2[] = {0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
		0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
		0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
		0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
		0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
		0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
		0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
	0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F};
	unsigned char *finalTestResult = selfTestResultV2;
	if(getFirmwareVer() == 0x91)//0x92 for version v2 , and 0x91 for v1
	{
		finalTestResult = selfTestResultV1;
	}
	//run perform soft reset
	resetRfid();
	//clear internal buffer
	writeToReg(FIFODataReg,0x00);
	writeToReg(CommandReg,MFRC522_MEM);
	//enable self test
	writeToReg(AutoTestReg,0x09);
	//Write 00h to the FIFO buffer
	writeToReg(FIFODataReg,0x00);
	//Start the self test with the CalcCRC command
	writeToReg(CommandReg,MFRC522_CALCCRC);
	
	//wait for self test to complete
	i= 0xFF;
	do 
	{
		crcInt = readFromReg(DivIEnReg);
		i--;
	} while ( (crcInt != 0x04) && (i != 0));
	
	//check test results
	for(i=0;i<64;i++)
	{
		if( readFromReg(FIFODataReg) != finalTestResult[i])
		{
			return False;//test fail
		}
	}
	
	//test passes
	return True;
}

void setRegBits(unsigned char addr,unsigned char mask)
{
	unsigned char current; 
	current= readFromReg(addr);
	current |= mask;
	writeToReg(addr,current);
}

void clearRegBits(unsigned char addr,unsigned char mask)
{
	unsigned char current;
	current= readFromReg(addr);
	current &= (~mask);
	writeToReg(addr,current);	
}

rfidError sendTagCommand(unsigned char cmd ,unsigned char *data,int dLen,unsigned char *result,int *rLen)
{
	rfidError status = MI_ERR;
	uint8_t irqEn = 0x00;
	uint8_t waitIRq = 0x00;
	uint8_t lastBits, n;
	int i;

	switch (cmd) {
		case MFRC522_AUTHENT:
		irqEn = 0x12;
		waitIRq = 0x10;
		break;
		case MFRC522_TRANSCEIVE:
		//softuart_puts(">>>MFRC522_TRANSCEIVE\n");
		irqEn = 0x77;
		waitIRq = 0x30;
		break;
		default:
		break;
	}

	//CommIEnReg Control bits to enable and disable
	// the passing of interrupt requests for IRQ pin
	writeToReg(CommIEnReg, irqEn|0x80);    // interrupt request
	//Interrupt request bits reg
	clearRegBits(CommIrqReg, 0x80);             // Clear all interrupt requests bits.
	//FIFOLevelReg Indicates the number of bytes stored in the FIFO.
	//clear internal fifo buffer
	setRegBits(FIFOLevelReg, 0x80);             // FlushBuffer=1, FIFO initialization.

	writeToReg(CommandReg, MFRC522_IDLE);  // No action, cancel the current command.

	//sprintf(tmp,"dlen : %d ,data[0] = %d \n\0",dLen,data[0]);
	//softuart_puts(tmp);
	
	// Write to FIFO
	for (i=0; i < dLen; i++) {
		writeToReg(FIFODataReg, data[i]);
		//softuart_puts("write to FIFODataReg 1");
	}

	// Execute the command.
	writeToReg(CommandReg, cmd);
	if (cmd == MFRC522_TRANSCEIVE) {
		//Adjustments for bit-oriented frames.
		//start transmission of data 
		setRegBits(BitFramingReg, 0x80);  // StartSend=1, transmission of data starts
		//softuart_puts("write to BitFramingReg 1");
	}
	//softuart_puts("here 1 \n");
	// Waiting for the command to complete so we can receive data.
	i = 25; // Max wait time is 25ms.
	do {
		//softuart_puts(" here 2 \n\0");
		_delay_ms(1);
		// CommIRqReg[7..0]
		// Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
		n = readFromReg(CommIrqReg);
		i--;
		//sprintf("i = %d \n\0",i);
		//softuart_puts(tmp);
	} while (( (i!=0) && !(n&0x01)) && !(n&waitIRq));
	clearRegBits(BitFramingReg, 0x80);  // StartSend=0
	//sprintf(tmp,"i = %d \n\0",i);
	//putsUsart0(tmp);
	//putsUsart0(" here 4 \n\0");
	
	if (i != 0) { // Request did not time out.
		if(!(readFromReg(ErrorReg) & 0x1B)) {  // BufferOvfl Collerr CRCErr ProtocolErr
			status = MI_OK;
			//putsUsart0("MI_OK ");
			if (n & irqEn & 0x01) {
				status = MI_NOTAGERR;
			}

			if (cmd == MFRC522_TRANSCEIVE) {
				n = readFromReg(FIFOLevelReg);
				lastBits = readFromReg(ControlReg) & 0x07;
				if (lastBits) {
					*rLen = (n-1)*8 + lastBits;
					} else {
					*rLen = n*8;
				}

				if (n == 0) {
					n = 1;
				}

				if (n > MAX_LEN) {
					n = MAX_LEN;
				}

				// Reading the recieved data from FIFO.
				for (i=0; i<n; i++) {
					result[i] = readFromReg(FIFODataReg);
				}
			}
			} else {
			status = MI_ERR;
			//putsUsart0("MI_ERR\n");
		}
	}
	return status;
}

rfidError scanForTag(unsigned char mode ,unsigned char *data)
{
		rfidError status;
		int len;
		writeToReg(BitFramingReg, 0x07);  // TxLastBists = BitFramingReg[2..0]

		data[0] = mode;
		status = sendTagCommand(MFRC522_TRANSCEIVE, data, 1, data, &len);

		if ((status != MI_OK) || (len != 0x10)) {
			
			if(status != MI_OK)
			{
				//putsUsart0("MI not ok\n");
			}
			status = MI_ERR;
		}

		return status;
}

void calculateCRC(uint8_t *data, int len, uint8_t *result)
{
	  int i;
	  uint8_t n;

	  clearRegBits(DivIrqReg, 0x04);   // CRCIrq = 0
	  setRegBits(FIFOLevelReg, 0x80);  // Clear the FIFO pointer

	  //Writing data to the FIFO.
	  for (i = 0; i < len; i++) {
		  writeToReg(FIFODataReg, data[i]);
	  }
	  writeToReg(CommandReg, MFRC522_CALCCRC);

	  // Wait for the CRC calculation to complete.
	  i = 0xFF;
	  do {
		  n = readFromReg(DivIrqReg);
		  i--;
	  } while ((i != 0) && !(n & 0x04));  //CRCIrq = 1

	  // Read the result from the CRC calculation.
	  result[0] = readFromReg(CRCResultRegL);
	  result[1] = readFromReg(CRCResultRegM);
}
rfidError haltTag()
{
	int status, len;
	uint8_t buffer[4];

	buffer[0] = MF1_HALT;
	buffer[1] = 0;
	calculateCRC(buffer, 2, &buffer[2]);
	clearRegBits(Status2Reg, 0x08);  // turn off encryption
	status = sendTagCommand(MFRC522_TRANSCEIVE, buffer, 4, buffer, &len);

	return status;
}

rfidError antiCollision(uint8_t *serial) {
	rfidError status, i, len;
	uint8_t check = 0x00;

	writeToReg(BitFramingReg, 0x00);  // TxLastBits = BitFramingReg[2..0]

	serial[0] = MF1_ANTICOLL;
	serial[1] = 0x20;
	status = sendTagCommand(MFRC522_TRANSCEIVE, serial, 2, serial, &len);
	len = len / 8; // len is in bits, and we want each uint8_t.
	if (status == MI_OK) {
		// The checksum of the tag is the ^ of all the values.
		for (i = 0; i < len-1; i++) {
			check ^= serial[i];
		}
		// The checksum should be the same as the one provided from the
		// tag (serial[4]).
		if (check != serial[i]) {
			status = MI_ERR;
		}
	}

	return status;
}

void dumpSerial(uint8_t*serial,uint8_t len ,unsigned char*dst)
{
	unsigned char tmp [len+1],i;
	for (i =0 ; i< len ;i++)
	{
		sprintf(dst+i*3,"%X.",serial[i]);
	}
	i = (i*3)-1;
	dst[i] = '\0';	
}