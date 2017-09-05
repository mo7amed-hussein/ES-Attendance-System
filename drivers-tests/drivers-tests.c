/*
* drivers_tests.c
*
* Created: 1/17/2018 11:23:48 AM
*  Author: mohamed
*/


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart/usart0.h"
#include "uart/softuart.h"
#include "wifi/wifi.h"
#include "ultrasonic/ultrasonic.h"
#include "dualultrasonic/dualultrasonic.h"
#include <stdio.h>
#include <string.h>
#define F_CPU 8000000UL

extern unsigned char wifiBuffer[WIFI_BUFFER_SIZE];
unsigned char tmp[WIFI_BUFFER_SIZE];
unsigned char tmp2[WIFI_BUFFER_SIZE];
volatile short counter =0;
volatile unsigned short left =20;
volatile unsigned short right =30;
volatile unsigned char data='0';
int main(void)
{
	sei();
	//initUsart0(9600);
	//initUltraSonic();
	
	softuart_init();
	
	initDualUltraSonic();
	
	//softuart_puts("ultrasonnic test start\n");
	//softuart_puts("ultrasonic initialized\n");
	
	//strcpy(wifiBuffer,"\0");
	
	/************************************************************************/
	/* wifi tests                                                                     */
	/************************************************************************/
	/*
	softuart_puts("wifi test start\n");
	softuart_puts("intialize wifi\n");
	if(initWifi()==False)
	{
		softuart_puts("wifi intialization error\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	softuart_puts("connecting to network\n");
	if(connectToNetwork("oka","mid0sexy") ==False)
	{
		softuart_puts("network connection error\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		softuart_puts(wifiBuffer);
		return 1;
	}
	softuart_puts("check Internet connection\n");
	if(checkInternet()==False)
	{
		softuart_puts("no internet access\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	
	
	softuart_puts("update data\n");
	if(updateData(65)==False)
	{
		softuart_puts("data update error\n");
		_delay_ms(1000);
		softuart_puts(getLastError());
		_delay_ms(1000);
		return 1;
	}
	*/
	//softuart_puts(wifiBuffer);
	DDRB |= (1<<PB1);
	
	
	//DDRC |= (1<<5);
	char ch='0';
	//softuart_putchar('a');
	//softuart_putchar('b');
	//softuart_putchar('\n');
	//softuart_puts("enable ultrasonic\n");
	//initUltraSonic();
	
	/* wifi tests */
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//_delay_ms(2000);
	//putsUsart0("AT\r\n");
	//_delay_ms(2000);
	//putsUsart0("AT+GMR\r\n");
	//_delay_ms(2000);
	////ch = getcUsart0();
	//softuart_puts("response");
	//softuart_putchar(ch);
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//putsUsart0("start test \n");
	char str[10];
	while(1)
	{
		/* usart0 tests */
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//putcUsart0('a');
		//putcUsart0('\n');
		//putsUsart0("wellcome to our app");
		//ch = getcUsart0();
		//putcUsart0(ch);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		
		/* software tests */
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//softuart_puts("test");
		//softuart_putchar('a');
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		
		/* ultrasonic tests */
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//softuart_puts("trigger ultrasonic\n");
		//triggerUltraSonic();
		//stopTimer();
		//counter=0;
		//softuart_puts("echo ultrasonic\n");
		//echoUltraSonic();
		//sprintf(str,"value is %d \n",counter);
		//softuart_puts(str);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//PORTC= (1<<5);
		//putsUsart0("AT+OK\r\n");
		//putcUsart0('A');
		//putcUsart0('\n');
		//putsUsart0("AT\r\n");
		//softuart_puts("read distance\n");
		_delay_ms(200);
		//counter = readDistance();
		readDistances(&left,&right);
		//softuart_puts("read distance done\n");
		//_delay_ms(1000);
		sprintf(tmp,"left :%d  , right : %d \n\0",left,right);
		softuart_puts(tmp);
		//softuart_puts(tmp);
		PORTB |= (1<<PB1);
		_delay_ms(500);
		PORTB &= ~(1<<PB1);
		_delay_ms(500);
		//softuart_putchar(data);
		//PORTC=0x00;
		//_delay_ms(500);
		
	}
}


ISR(USART_RX_vect)
{
	int bufferLen =strlen(wifiBuffer);
	wifiBuffer[bufferLen] = UDR0;
	//softuart_putchar(UDR0);
}
