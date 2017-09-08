/*
 * wifi_test.c
 *
 * Created: 1/25/2018 9:50:02 PM
 *  Author: mohamed
 */ 


#include <avr/io.h>
#include "util/delay.h"
#include "avr/interrupt.h"
#include "wifi/wifi.h"
#include "uart/usart0.h"
#include "uart/softuart.h"
extern unsigned char wifiBuffer[WIFI_BUFFER_SIZE];
unsigned char buffer[50];
volatile uint8_t data = 34 ;
int main(void)
{
	sei();
	//intialize usart0 
	//initUsart0(9600);
	//init software uart
	softuart_init();
	softuart_puts("Wifi test starts...\n");
	//init wifi chip
	if(initWifi()==False)
	{
		softuart_puts("wifi intialization error...\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	softuart_puts("connecting to network...\n");
	if(connectToNetwork("oka","mid0sexy") ==False)
	{
		softuart_puts("network connection error\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		softuart_puts(wifiBuffer);
		return 1;
	}
	softuart_puts("check Internet connection...\n");
	if(checkInternet()==False)
	{
		softuart_puts("no internet access\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	
	softuart_puts("update data...\n");
	if(updateData(data)==False)
	{
		softuart_puts("data update error\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	softuart_puts("new data updated succeffully...\n");
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
	
	
	uint8_t counter = 0;
	uint8_t times = 0;
	data++;
    while(1)
    {
        //TODO:: Please write your application code 
		if(counter == 15 && times <5)
		{
			softuart_puts("update data...\n");
			if(updateData(data)==False )
			{
				softuart_puts("data update error\n");
				_delay_ms(1000);
				softuart_puts(getLastError());
				_delay_ms(1000);
				return 1;
			}
			softuart_puts("new data updated succeffully...\n");
			counter =0;
			times++;
		}
		counter++;
		//blink system led
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }
}

ISR(USART_RX_vect)
{
	int bufferLen =strlen(wifiBuffer);
	wifiBuffer[bufferLen] = UDR0;
	//softuart_putchar(UDR0);
}