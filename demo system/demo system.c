
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

int main(void)
{
	//initialize softuart
	softuart_init();
	//initialize PIR motion detector
	initPir();
	_delay_ms(1000);
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
    while(1)
    {
       
		
		//blink system led
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }
}