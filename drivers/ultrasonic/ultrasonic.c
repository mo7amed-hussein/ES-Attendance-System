/*
* Ultrasonic.c
*
* Created: 28/08/2016 05:07:31 ã
*  Author: tofaa
*/
#include "Ultrasonic.h"
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned short overflows = 0;

void initUltraSonic(void){
	/* init io */
	US_PORT_DDR |=  (1<<TRIGGER_PIN); /* TRIG */
	US_PORT		&= ~(1<<TRIGGER_PIN); /* TRIG */
	US_PORT_DDR &= ~(1<<ECHO_PIN); /* ECHO */
	US_PORT		&= ~(1<<ECHO_PIN); /* ECHO */
	
	/* init timer0 */
	
	TCCR1B |= (1<<CS11); /* 1/8 Prescaler */
	TIMSK1 |= (1<<TOIE1); /* Enable Overflow Int */
	sei();				/* Enable Global interrupt */
}
unsigned short readDistance(void){
	unsigned short pulse_width = 0,distance = 0;
	/* Send Trigger */
	US_PORT |= (1<<TRIGGER_PIN);
	_delay_us(10);
	US_PORT &= ~(1<<TRIGGER_PIN);
	/* Measure Pulse Width */
	while(!(US_PORT_PIN & (1<<ECHO_PIN))); /* Wait for rising edge */
	TCNT1 = 0;
	overflows = 0;
	while((US_PORT_PIN & (1<<ECHO_PIN)))/* Wait for falling edge */
	{
		if (overflows ==1) // maximum is 23 cm
		{
			break;
		}
	}
	pulse_width = TCNT1 + overflows*65535; /* Calc, Pulse Width in us */
	distance = (pulse_width/29.1)/2; /* distance in cm */
	return distance;
}
ISR(TIMER1_OVF_vect){
	overflows++;
}