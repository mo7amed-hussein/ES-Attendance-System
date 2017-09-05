/*
* Ultrasonic.c
*
* Created: 28/08/2016 05:07:31 ã
*  Author: tofaa
*/
#include "dualultrasonic.h"
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned short overflows = 0;

void initDualUltraSonic(void){
	/* init io */
	US_PORT_DDR |=  (1<<TRIGGER_PIN); /* TRIG */
	US_PORT		&= ~(1<<TRIGGER_PIN); /* TRIG */
	
	//right echo
	US_PORT_DDR &= ~(1<<RIGHT_ECHO); /* ECHO */
	US_PORT		&= ~(1<<RIGHT_ECHO); /* ECHO */
	
	//left echo 
	US_PORT_DDR &= ~(1<<LEFT_ECHO); /* ECHO */
	US_PORT		&= ~(1<<LEFT_ECHO); /* ECHO */
	
	/* init timer1 */
	
	TCCR1B |= (1<<CS11); /* 1/8 Prescaler */
	TIMSK1 |= (1<<TOIE1); /* Enable Overflow Int */
	sei();				/* Enable Global interrupt */
}
void readDistances(unsigned short*left,unsigned short*right){
	char isLeftDone=0 , isRightDone =0;
	unsigned short righTime =0 , leftTime =0;
	/* Send Trigger */
	US_PORT |= (1<<TRIGGER_PIN);
	_delay_us(10);
	US_PORT &= ~(1<<TRIGGER_PIN);
	/* Measure Pulse Width */
	while(!(US_PORT_PIN & (1<<RIGHT_ECHO))); /* Wait for rising edge */
	TCNT1 = 0;
	overflows = 0;
	while( (isRightDone == 0) || (isLeftDone == 0)  )/* Wait for falling edge */
	{
		if( (isRightDone== 0) && (!(US_PORT_PIN & (1<<RIGHT_ECHO))) )
		{
			isRightDone = 1;
			righTime = TCNT1 + overflows*65535;
		}
		if( (isLeftDone== 0) && (!(US_PORT_PIN & (1<<LEFT_ECHO))) )
		{
			isLeftDone = 1;
			leftTime = TCNT1 + overflows*65535;
		}
		if (overflows ==5) // maximum is 23 cm
		{
			break;
		}
	}
	//pulse_width = TCNT1 + overflows*65535; /* Calc, Pulse Width in us */
	//distance = (pulse_width/29.1)/2; /* distance in cm */
	(*right) = (righTime/29.1)/2;
	(*left) = (leftTime/29.1)/2;
}
ISR(TIMER1_OVF_vect){
	overflows++;
}