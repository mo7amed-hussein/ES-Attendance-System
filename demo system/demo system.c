
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

int main(void)
{
	//initialize softuart
	softuart_init();
	//initialize PIR motion detector
	initPir();
	_delay_ms(1000);
	//initialize audience counter
	initDualUltraSonic();
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
	while(1)
	{
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
		
		
		//blink system led
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
	}
}