/*
 * TimerTest.c
 *
 *  Created on: 06.02.2012
 *      Author: paul
 * Description: This Programm does toggle Port D Pin 6 all 20ms when the Microcontroller has an 16MHZ Oscillator
 */
#import <avr/io.h>
#import <avr/interrupt.h>
uint8_t servo1;
uint8_t nextServo;
uint16_t usedTime;
#define SERVOCOUNT 1
int main(int argc, char** argv)
{
	DDRD |= (1 << DDD7) | (1 << DDD5);
	PORTD |= (1 << PD7) | (1 << PD5);

	TCCR1A = (0 << WGM10) | (0 << WGM11); //WGM10 - 13 ctc
	TCCR1B = (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10); // divide system clock by 64
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 5000;
	usedTime = 0;
	servo1 = 0;
	sei();

	while(1){/*do not finish*/}
	return 0;
}
ISR(TIMER1_OVF_vect){}
ISR(TIMER1_COMPA_vect)
{
    char tmp = SREG; //backup SREG state
    cli();
    //do something that ist not atomic
    //PORTD ^= (1 << DD7); //toggle pin 7
    switch(nextServo)
    {
    case 0:
    	PORTD |= (1 << PD7);
    	OCR1A = 0x100| servo1;
    	break;
    case 1:
    	PORTD &= ~(1 << PD7);
    	OCR1A = 5000 - usedTime;

    }
    usedTime += OCR1A;
    nextServo++;
    if(nextServo > SERVOCOUNT)
    {
    	usedTime = 0;
    	nextServo = 0;
    }

    SREG = tmp;//Restore SREG
}

