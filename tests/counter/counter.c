#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart/usart0.h"
//#include "dualultrasonic/dualultrasonic.h"
//define F_CPU
#ifndef F_CPU
#define F_CPU 8000000ul
#endif

#define PASSAGE_WIDTH 20 // in cm
#define ULTRASONIC_TOLERANCE  2 // sensor tolerance  in cm
#define COUNTER_DELAY 1000
unsigned short leftDistance =0;
unsigned short rightDistance =0;

volatile uint8_t isLeft = 0x00;
volatile uint8_t isRight = 0x00;
volatile unsigned short audience = 0;
unsigned char buffer[50];
int main(void)
{
	
	initUsart0(9600);
	_delay_ms(100);
	putsUsart0("dual ultrasonic test\n");
	initDualUltraSonic();
	//system led
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);

	//enable global interrupt
	//sei();
    while(1)
    {
        //TODO:: Please write your application code
		//blink system led
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
		//sprintf(buffer,"L : %d , R : %d \n\0",leftDistance,rightDistance);
		//putsUsart0(buffer);
		PORTB |= (1<<PB0);
		_delay_ms(500);
		PORTB &= ~(1<<PB0);
		_delay_ms(500);
    }
}