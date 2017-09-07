/*
 * pir.h
 *
 * Created: 1/23/2018 2:45:42 PM
 *  Author: mohamed
 */ 


#ifndef PIR_H_
#define PIR_H_

#define PIR_PORT PORTD
#define PIR_PIN PIND7
#define PIR_PORT_DDR DDRD
#define PIR_PIN_IN PIND

#define Light_PORT PORTB
#define Light_PIN PINB6
#define Light_PORT_DDR DDRB

void initPir();
void turnLight();

#endif /* PIR_H_ */