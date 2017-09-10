
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart/usart0.h"
#include "uart/softuart.h"
#include "wifi/wifi.h"
#include "dualultrasonic/dualultrasonic.h"
#include "rfid/rfid.h"
#include "pir/pir.h"
#include <stdio.h>
#include <string.h>
#define F_CPU 8000000UL

#define PASSAGE_WIDTH 20 // passage width in cm
#define ULTRASONIC_TOLERANCE  2 // sensor tolerance  in cm
#define COUNTER_DELAY 1000 //delay between right and left sensor ,for each sensor coverage area
unsigned short leftDistance =0;//left sensor distance in cm
unsigned short rightDistance =0;//right sensor distance in cm

volatile uint8_t isLeft = 0x00; //left sensor flag
volatile uint8_t isRight = 0x00;//right sensor flag
volatile unsigned short audience = 0;//audience counter
unsigned char buffer[50]; //used for debugging on softuart

extern unsigned char wifiBuffer[WIFI_BUFFER_SIZE]; //wifi buffer 

unsigned char *serials[] = {"12.4C.6E.8B","FF.FF.FF.FF.FF"};//validated tags ids
uint8_t serialsCount =2;//# of validated tags ids
unsigned char tmpSerial [12]; //for dump serial form hex to string

int main(void)
{
	//initialize softuart
	softuart_init();
	//initialize PIR motion detector
	initPir();
	_delay_ms(1000);
	//initialize audience counter
	initDualUltraSonic();
	//init wifi chip
	if(initWifi()==False)
	{
		//softuart_puts("wifi intialization error...\n");
		_delay_ms(1000);
		//softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	//softuart_puts("connecting to network...\n");
	if(connectToNetwork("oka","mid0sexy") ==False)
	{
		//softuart_puts("network connection error\n");
		_delay_ms(1000);
		//softuart_puts(getLastError());
		_delay_ms(1000);
		//softuart_puts(wifiBuffer);
		return 1;
	}
	//softuart_puts("check Internet connection...\n");
	if(checkInternet()==False)
	{
		//softuart_puts("no internet access\n");
		_delay_ms(1000);
		//softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	
	//softuart_puts("update data...\n");
	if(updateData(audience)==False)
	{
		//softuart_puts("data update error\n");
		_delay_ms(1000);
		//softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	//softuart_puts("new data updated succeffully...\n");
	
	initRfid();
	_delay_ms(1000);
	uint8_t ver = getFirmwareVer();
	if(!ver)
	{
		//softuart_puts("Didn't find MFRC522 board.");
		//return;
		while(1);
	}
	
	//softuart_puts("Found chip MFRC522\n");
	//softuart_puts("firmware version is ");
	sprintf(buffer,"%X \n\0",ver);
	//softuart_puts(buffer);
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
	
	unsigned int wifiTimer = 0;
	while(1)
	{
		uint8_t status; //rfid error status
		uint8_t data[MAX_LEN];//for rfid tags search
		uint8_t serial[5];//tag id in hex
		
		/************************************************************************/
		/* audience counter                                                     */
		/************************************************************************/
		readDistances(&leftDistance,&rightDistance);
		if(rightDistance <(PASSAGE_WIDTH-ULTRASONIC_TOLERANCE))
		{
			if(isLeft == 0x01)
			{
				audience++;
				isLeft=0x00;
			}
			else
			{
				isRight = 0x01;
				
			}
			_delay_ms(COUNTER_DELAY);
		}
		
		if(leftDistance <(PASSAGE_WIDTH-ULTRASONIC_TOLERANCE))
		{
			if(isRight == 0x01)
			{
				audience--;
				isRight=0x00;
			}
			else
			{
				isLeft = 0x01;
				
			}
			_delay_ms(COUNTER_DELAY);
		}
		sprintf(buffer,"L : %d , R : %d , audience : %d \n\0",leftDistance,rightDistance,audience);
		putsUsart0(buffer);
		
		/************************************************************************/
		/* update audience count for online use                                 */
		/************************************************************************/
		
		if(wifiTimer == 60 )//update each minute as loop has 1 sec delay below
		{
			//softuart_puts("update data...\n");
			if(updateData(audience)==False )
			{
				//softuart_puts("data update error\n");
				_delay_ms(1000);
				//softuart_puts(getLastError());
				_delay_ms(1000);
				return 1;
			}
			//softuart_puts("new data updated succeffully...\n");
			wifiTimer =0;
		}
		wifiTimer++;
		
		/************************************************************************/
		/* control access                                                       */
		/************************************************************************/
		//softuart_puts("scannnig for tags ... \n");
		//status = requestTag(MF1_REQIDL, data);
		status = scanForTag(MF1_REQIDL, data);
		if(status == MI_OK )
		{
			//softuart_puts("tag detected \n");
			sprintf(buffer,"type %X , %X \n\0",data[0],data[1]);
			putsUsart0(buffer);
			status = antiCollision(data);
			memcpy(serial, data, 5);
			//
			//softuart_puts("The serial nb of the tag is:");
			//for (int i = 0; i < 4; i++) {
			//sprintf(tmp,"%X , \0",serial[i]);
			//putsUsart0(tmp);
			//}
			dumpSerial(serial,4,tmpSerial);
			//softuart_puts(tmpSerial);
			for(int i = 0 ;i<serialsCount;i++)
			{
				if(strcmp(tmpSerial,serials[i])==0)
				{
					//softuart_puts("\nauthenticated user\n");
				}
			}
			haltTag();
		}
		
		//blink system led
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
	}
}