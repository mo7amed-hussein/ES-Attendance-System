


#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "spi/spi.h"
#include "rfid/rfid.h"
#include "uart/softuart.h"

unsigned char *serials[] = {"12.4C.6E.8B","FF.FF.FF.FF.FF"};
uint8_t serialsCount =2;
unsigned char tmpSerial [12];
unsigned char tmp[WIFI_BUFFER_SIZE];
unsigned char buffer[50];
int main(void)
{
	//initUsart0(9600);
	sei();
	softuart_init();
	initRfid();
	_delay_ms(1000);
	uint8_t ver = getFirmwareVer();
	if(!ver)
	{
		softuart_puts("Didn't find MFRC522 board.");
		//return;
		while(1);
	}
	
	softuart_puts("Found chip MFRC522\n");
	softuart_puts("firmware version is ");
	sprintf(buffer,"%X \n\0",ver);
	softuart_puts(buffer);
	
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
    while(1)
    {
        uint8_t status;
        uint8_t data[MAX_LEN];
        uint8_t serial[5];
        softuart_puts("scannnig for tags ... \n");
        //status = requestTag(MF1_REQIDL, data);
        status = scanForTag(MF1_REQIDL, data);
        if(status == MI_OK )
        {
	        softuart_puts("tag detected \n");
	        sprintf(tmp,"type %X , %X \n\0",data[0],data[1]);
	        putsUsart0(tmp);
	        status = antiCollision(data);
	        memcpy(serial, data, 5);
	        //
	        softuart_puts("The serial nb of the tag is:");
	        //for (int i = 0; i < 4; i++) {
	        //sprintf(tmp,"%X , \0",serial[i]);
	        //putsUsart0(tmp);
	        //}
	        dumpSerial(serial,4,tmpSerial);
	        softuart_puts(tmpSerial);
	        for(int i = 0 ;i<serialsCount;i++)
	        {
		        if(strcmp(tmpSerial,serials[i])==0)
		        {
			        softuart_puts("\nauthenticated user\n");
		        }
	        }
	        haltTag();
        }
        
        //data++;
        PORTB |= (1<<PB0);
        _delay_ms(1000);
        PORTB &= ~(1<<PB0);
        _delay_ms(1000);
        //softuart_putchar(data);
        //PORTC=0x00;
        //_delay_ms(500);
        
        }
}