/*
 * spi.c
 *
 * Created: 1/19/2018 9:23:27 PM
 *  Author: mohamed
 */ 
#include <avr/io.h>
#include "spi.h"

void initSpiMaster()
{
	//set MOSI , SCK as output
	SPI_PORT_DDR |= (1<<SPI_MOSI_PIN) | (1<<SPI_SCK_PIN) | (1<<SPI_SS_PIN);
	//config as master
	SPCR = (1<<MSTR);
	//enable transfer interrupt
	//SPCR |= (1<<SPIE);
	//enable spi
	SPCR |= (1<<SPE);
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
	//wait until transmission complete
	while(!(SPSR&(1<<SPIF)));
	//return received data
	return SPDR;
}