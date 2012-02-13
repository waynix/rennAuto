/*
 * TimerTest.c
 *
 *  Created on: 06.02.2012
 *      Author: paul
 * Description: This Programm does toggle Port D Pin 6 all 20ms when the Microcontroller has an 16MHZ Oscillator
 */
#import <avr/io.h>
#import <avr/interrupt.h>
#import <util/delay.h>
volatile uint8_t servo1;
uint8_t nextServo;
uint16_t usedTime;
#define SERVOCOUNT 1
int main(int argc, char** argv)
{
	//general config
	//set output pins
	//Servo Ports
	DDRD |= (0 << DDD0)
		  | (1 << DDD1)
		  | (1 << DDD7)
		  | (1 << DDD5);
	PORTD = (1 << PD5);

	//WGM10 - 13 ctc
	TCCR1A = (0 << WGM10) | (0 << WGM11);
	TCCR1B = (0 << WGM13) | (1 << WGM12);
	// divide system clock by 64
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	TIMSK1 = (1 << OCIE1A);

	//            Reference Voltage       left Aligned     Source Pin PORT A 0
	ADMUX = (0 << REFS0) | (0 << REFS1)   | (1 << ADLAR) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	//       Enable      Start Conversion   Auto Trigger   ADC Interrupt CPU Clock Divider(prescaler)
	ADCSRA = (1 << ADEN) | (1 << ADSC)    | (0 << ADATE) | (1 << ADIE) | (0 << ADPS2)| (0 << ADPS1)| (0 << ADPS0);
	OCR1A = 5000;


	//USART configuration
//	//clear configuration registers
//	UCSR0A = 0;
//	UCSR0B = 0;
//	UCSR0C = 0;
	//Baud Rate 768000Bit/s (at 16MHZ clock)
	UBRR0H = (unsigned char)0;
	UBRR0L = (unsigned char)25;
	//Double Data Rate
	UCSR0A  = (0 << U2X0)
	//Multiprocessor Communication Mode
			| (0 << MPCM0);
	//State: (1= on 0 =off)
	UCSR0B =  (1 << RXEN0) //Receiver
			| (1 << TXEN0)//Transmitter
	//Interrupts:(1= on 0 =off)
			| (1 << RXCIE0) //Receive
			| (0 << TXCIE0) //Transmit
			| (0 << UDRIE0) //Send buffer free
	//Bit Count:8
		    | (0 << UCSZ02);
	UCSR0C  = (1 << UCSZ01)
			| (1 << UCSZ00)
    //CRC Mode: even
			|  (1 << UPM01)
			| (0 << UPM00)
	//Stop Bits: 1bit = 0 2bits = 1
			| (0 << USBS0)
	//USART Mode: asynchron
			| (0 << UMSEL01)
			| (0 << UMSEL00);


	//clear
	usedTime = 0;
	servo1 = 0;
	sei();


	while(1){/*do not finish*/

	}
	return 0;
}
ISR(ADC_vect)
{
	servo1 = ADCH;
}
ISR(USART_RX_vect)
{
	volatile uint8_t tmp = UDR0;
	UDR0 = tmp;
	PORTD ^= (1 << PD5);
	servo1 = tmp;
}
ISR(TIMER1_OVF_vect){}
ISR(TIMER1_COMPA_vect)
{
    char tmp = SREG; //backup SREG state
    cli();

    switch(nextServo)
    {
    case 0:
    	PORTD |= (1 << PD7);
    	OCR1A = 0x100| servo1;

    	break;
    case SERVOCOUNT:
    	PORTD &= ~(1 << PD7);
    	OCR1A = 5000 - usedTime;
    	break;
    default:
    	break;
    }
    usedTime += OCR1A;
    nextServo++;
    if(nextServo > SERVOCOUNT)
    {
    	usedTime = 0;
    	nextServo = 0;
    	//measure next Voltage value
    	//ADCSRA |= (1 << ADSC);
    	//PORTD ^= (1 << PD5);
    }

    SREG = tmp;//Restore SREG
}

