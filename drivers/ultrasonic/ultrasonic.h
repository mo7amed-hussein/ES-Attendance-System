#ifndef _ULTRASONIC_
#define _ULTRASONIC_
#include <avr/io.h>


#define TRIGGER_PIN  PD7
#define ECHO_PIN  PD6

#define ULTRA_PORT PORTD
#define ULTRA_PORT_DDR DDRD
#define ULTRA_PORT_PIN PIND

void initUltraSonic();
void triggerUltraSonic();
void echoUltraSonic();
void startTimer();
void stopTimer();



#endif