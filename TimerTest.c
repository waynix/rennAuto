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
	TCCR1A = (0 << WGM10) || (1 << WGM11);
	TCCR1B = (0 << WGM13) || (0 << WGM12) || (0 << CS10) || (1 << CS11) || (0 << CS12); //
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

