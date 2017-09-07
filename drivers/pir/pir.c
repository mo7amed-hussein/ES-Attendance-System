#include "pir.h"
#include "../uart/usart0.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void initPir()
{
	//set pir pin as input
	PIR_PORT_DDR &= ~(1<<PIR_PIN);
	//set light as output
	Light_PORT_DDR |= (1<<Light_PIN);
	Light_PORT |= (1<<Light_PIN);
	//config interrupt
	PCICR = (1<<PCIE2);
	PCMSK2 = (1<<PCINT23); 
	sei();
}

void turnLight()
{
	putsUsart0("\nON\n");
}

ISR(PCINT2_vect)
{
	//putsUsart0("int");
	if( PIR_PIN_IN & (1<<PIR_PIN))
	{
		turnLight();
	}
}