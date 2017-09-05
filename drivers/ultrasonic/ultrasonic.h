#ifndef _ULTRASONIC_
#define _ULTRASONIC_
#include <avr/io.h>


#define TRIGGER_PIN  PIND3
#define ECHO_PIN  PIND4

#define US_PORT PORTD
#define US_PORT_DDR DDRD
#define US_PORT_PIN PIND

void initUltraSonic(void);
unsigned short readDistance(void);



#endif