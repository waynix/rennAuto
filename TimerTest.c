/*
 * TimerTest.c
 *
 *  Created on: 06.02.2012
 *      Author: paul
 * Description: This Programm does toggle Port D Pin 6 all 20ms when the Microcontroller has an 16MHZ Oscillator
 */
#import <avr/io.h>
#import <avr/interrupt.h>
int main()
{
	DDRD = (1 << DD6);
	PORTD = (0 << DD6);
	TCCR1A = (0 << WGM10) || (0 << WGM11); //WGM10 - 13 ctc
	TCCR1B = (0 << WGM13) || (1 << WGM12) || (1 << CS10) || (1 << CS11) || (0 << CS12); // divide system clock by 64
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 5000;//diese aktion ist nicht atomar(16bit) und greif auf timersachen zu deswegen die interrupts erst danach setzen
	sei();

	while(1){}
	return 0;
}
ISR(TIMER1_COMPA_vect)
{
    PORTD ^= (1 << DD6); //toggle pin 6
//    char tmp = SREG; //backup SREG state
//    cli();
//    //do something that ist not atomic
//    SREG = tmp;//Restore SREG
}

