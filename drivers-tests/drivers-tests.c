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
#include "ultrasonic/ultrasonic.h"
#include <stdio.h>
#include <string.h>
#define F_CPU 8000000UL

volatile short counter =0;
int main(void)
{
	initUsart0(9600);
	sei();
	softuart_init();
	
	//DDRC |= (1<<5);
	char ch='0';
	
	softuart_puts("enable ultrasonic\n");
	initUltraSonic();
	
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
		softuart_puts("trigger ultrasonic\n");
		triggerUltraSonic();
		stopTimer();
		counter=0;
		softuart_puts("echo ultrasonic\n");
		echoUltraSonic();
		sprintf(str,"value is %d \n",counter);
		softuart_puts(str);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		//PORTC= (1<<5);
		_delay_ms(500);
		//PORTC=0x00;
		//_delay_ms(500);
		
    }
}


ISR(TIMER0_OVF_vect)
{
	counter++;	
}