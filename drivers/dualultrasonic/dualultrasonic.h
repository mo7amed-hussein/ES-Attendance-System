#ifndef _DUALULTRASONIC_
#define _DUALULTRASONIC_
#include <avr/io.h>


#define TRIGGER_PIN  PIND3
#define RIGHT_ECHO  PIND2
#define LEFT_ECHO  PIND4

#define US_PORT PORTD
#define US_PORT_DDR DDRD
#define US_PORT_PIN PIND

void initDualUltraSonic(void);
void readDistances(unsigned short*left,unsigned short*right);



#endif