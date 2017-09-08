#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart/usart0.h"
#include "pir/pir.h"
//define F_CPU
#ifndef F_CPU
#define F_CPU 16000000ul
#endif

int main(void)
{
	//initialize uart
	initUsart0(9600);
	_delay_ms(200);
	//initialize pir
	initPir();
	_delay_ms(1000);
	
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
	//enable global interrupt
	sei();
	
	putsUsart0("PIR Test Start... \n");
	
    while(1)
    {
        //TODO:: Please write your application code
		//blink system led
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }
}