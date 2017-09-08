/*
 * spi.c
 *
 * Created: 1/19/2018 9:23:27 PM
 *  Author: mohamed
 */ 
#include <avr/io.h>
#include "spi.h"
#include "../uart/softuart.h"

unsigned char tmp[10];
void initSpiMaster()
{
	//set MOSI , SCK as output
	SPI_PORT_DDR |=  (1<<SPI_SS_PIN);
	//SPI_PORT &= ~(1<<SPI_MISO_PIN);
	//config as master , MSB first
	//SPCR = (1<<MSTR)|(0<<DORD);
	SPCR = (1<<MSTR);
	//set data mode to mode 0
	//SPCR |= (0x0C & 0x00);
	//set clock to fosc/2 
	//SPCR |= (0x03 & 0x00);
	//set double spi speed
	//SPSR =  0x00;
	//enable transfer interrupt
	//SPCR |= (1<<SPIE);
	//SPCR |= (1<<SPI2X); 
	//enable spi
	SPCR |= (1<<SPE);
	SPI_PORT_DDR |= (1<<SPI_MOSI_PIN) | (1<<SPI_SCK_PIN) | (1<<SPI_SS_PIN);
}

void initSpiSlave()
{
	//config MISO as output
	SPI_PORT_DDR |= (1<<SPI_MISO_PIN);
	//enable spi
	SPCR |= (1<<SPE);
}

unsigned char tranceiveSpi(unsigned char data)
{
	//load data into buffer
	SPDR = data;
	//sprintf(tmp,"data = %X \n\0",data);
	//softuart_puts(tmp);
	//wait until transmission complete
	 asm volatile("nop");
	while(!(SPSR&(1<<SPIF)));
	//return received data
	return SPDR;
}