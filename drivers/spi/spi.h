/*
 * spi.h
 *
 * Created: 1/19/2018 9:23:13 PM
 *  Author: mohamed
 */ 


#ifndef SPI_H_
#define SPI_H_

#define SPI_PORT_DDR DDRB
#define SPI_MOSI_PIN  PINB3
#define SPI_MISO_PIN  PINB4
#define SPI_SCK_PIN  PINB5
#define SPI_SS_PIN  PINB2
#define SPI_PORT PORTB
#define ACK 0x7E
void initSpiMaster();

void initSpiSlave();

unsigned char tranceiveSpi(unsigned char data);

#endif /* SPI_H_ */