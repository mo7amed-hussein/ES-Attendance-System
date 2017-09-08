#include "pir.h"
#include "../uart/usart0.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t currentState = 0x00;
void initPir()
{
	//set pir pin as input
	PIR_PORT_DDR &= ~(1<<PIR_PIN);
	//set light as output
	Light_PORT_DDR |= (1<<Light_PIN);
	Light_PORT &= ~(1<<Light_PIN);
	//config interrupt
	PCICR = (1<<PCIE2);
	PCMSK2 = (1<<PCINT23);
	currentState =0x00;
	sei();
}

void turnLight()
{
	if(currentState==0x01)
	{
		putsUsart0("Already on... \n");
	}
	else
	{
		putsUsart0("Turn Light on ...\n");
		Light_PORT |= (1<<Light_PIN);
		currentState =0x01;
	}
	
	
}

ISR(PCINT2_vect)
{
	if( PIR_PIN_IN & (1<<PIR_PIN))
	{
		turnLight();
	}
}