#include "usart0.h"
#include <avr/io.h>

void initUsart0(unsigned short baudrate)
{
	
	//set baudrate
	unsigned short ubbrVal= (FCLK_SYSTEM/(baudrate*16UL))-1;
	UBRR0H = (unsigned char )(ubbrVal>>8);
	UBRR0L = (unsigned char )(ubbrVal);
	
	//set frame format : async , no parity , 8 bit size , 1 stop bit
	UCSR0C = (0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)|(3<<UCSZ00);
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	//config for transmit and recieve	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
}

void putcUsart0(unsigned char data)
{
	/* Wait for empty transmit buffer*/
	while( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data*/
	UDR0 = data;
	
}

void putsUsart0(char *str)
{
	short int i=0;
	while(str[i])
	{
		putcUsart0(str[i]);
		i++;
	}
}

unsigned char getcUsart0(void)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}