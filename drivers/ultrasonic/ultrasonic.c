#include "ultrasonic.h"
#include "../uart/softuart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void initUltraSonic()
{
	//set trigger , ground pins as outputs
	ULTRA_PORT_DDR |= (1<<TRIGGER_PIN)| (0<<ECHO_PIN);
	//set echo pin as input
	//ULTRA_PORT_DDR &=  ~(1<<ECHO_PIN);
	//set ground pin equal 0
	//ULTRA_PORT &= ~(1<<GND_PIN);
	ULTRA_PORT_PIN |= (1<<ECHO_PIN);
}

void triggerUltraSonic()
{
	softuart_puts("output hight to trigger\n");
	ULTRA_PORT &= ~(1<<TRIGGER_PIN);
	_delay_us(2);
	ULTRA_PORT |= (1<<TRIGGER_PIN);
	_delay_us(10);
	ULTRA_PORT &= ~(1<<TRIGGER_PIN);
	softuart_puts("output low to trigger\n");
}

void echoUltraSonic()
{
	softuart_puts("wait for echo to go high\n");
	while(!(ULTRA_PORT_PIN & (1<<ECHO_PIN)));
	startTimer();
	softuart_puts("wait for echo to go low\n");
	while((ULTRA_PORT_PIN & (1<<ECHO_PIN)));
	softuart_puts("echo went low\n");
	stopTimer();
}

void startTimer()
{	
	TCCR0A|=(1<<CS01);			//clkI/8
	TIMSK0|=(1<<TOIE0);			//enable timer0
	TCNT0=0;					//zero timer0
	sei();						//open global interrupt
}

void stopTimer()
{
	TCCR0A &= ~(0<<CS01);			//stop timer
}