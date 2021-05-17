

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"




void INT0_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD2));               // Configure INT0/PD2 as input pin
	PORTD |= (1<<PD2);   				// Enable the internal pull up resistor at PD2 pin
	GICR  |= (1<<INT0);                 // Enable external interrupt pin INT0
	MCUCR |= (1<<ISC01);  				// Trigger INT0 with the falling edge
	MCUCR &= ~(1<<ISC00);
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void INT1_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRD  &= (~(1<<PD3));               // Configure INT1/PD3 as input pin
	GICR  |= (1<<INT1);                 // Enable external interrupt pin INT1
	MCUCR |= (1<<ISC11) | (1<<ISC10);   // Trigger INT1 with the raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void INT2_Init(void)
{
	SREG  &= ~(1<<7);                   // Disable interrupts by clearing I-bit
	DDRB   &= (~(1<<PB2));  			// Configure INT2/PB2 as input pin
	PORTB |= (1<<PB2);   				// Enable the internal pull up resistor at PB2 pin
	GICR   |= (1<<INT2);				// Enable external interrupt pin INT2
	MCUCSR &= (~(1<<ISC2));     		// Trigger INT2 with the Falling edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

void Timer1_CTC_Init(void)
{
	/* Configure timer control register
	 * 1. clock = 1024 prescaler CS10=1 CS11=0 CS12=1
	 * 2. Normal port operation, OC1A/OC1B disconnected  COM1A1/COM1B1 COM1A0/COM1B0 =0
	 * 3 . FOC1A one PWM
	 */
	TCCR1A = (1<<FOC1A)  ;
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS12);
	TCNT1 = 0;		//start

	OCR1A=976;		//compair
	TIMSK=(1<<OCIE1A);  //enabling the compare match interrupt


	/// if(TIFR&(1<<OCF1A))



}
ISR(TIMER1_COMPA_vect)
{
	 TIME1_Flag = 1;

}

ISR(INT0_vect)
{
	// set the interrupt flag to indicate that INT0 occurs
	INT0_Flag = 1;
}

ISR(INT1_vect)
{
	// set the interrupt flag to indicate that INT1 occurs
	INT1_Flag = 1;


}

ISR(INT2_vect)
{
	// set the interrupt flag to indicate that INT2 occurs
	INT2_Flag = 1;
}

void clock_init()
{
	//PORTC &= 0xF0;

	sec0=0;
	sec1=0;
	min0=0;
	min1=0;
	hour0=0;
	hour1=0;

}
void resume_time1_CTC(){
	PORTC &= 0xF0;
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS12);

}
void reset()
{
	clock_init();
	//resume_time1_CTC();
}
void pause_timer1()
{
	TCCR1B &= 0B11111000;
	//TCCR1B &= ~ ((1<<CS12) | (1<<CS12) | (1<<CS12));
}
void incsec0()
{
	if (sec0==9)
	{
		sec0=0;

		incsec1();
	}
	else
	{
		sec0++;

	}

}

void incsec1()
{
	if (sec1==5)
	{
		sec1=0;
		incmin0();
	}
	else
	{
		sec1++;

	}

}
void incmin0()
{

	if (min0==9)
	{
		min0=0;

		incmin1();
	}
	else
	{
		min0++;

	}


}
void incmin1()
{

	if (min1==5)
	{
		min1=0;

		inchour0();
	}
	else
	{
		min1++;


	}


}
void inchour0(){

	if (hour0==9)
	{
		hour0=0;

		inchour1();
	}
	else if (hour0==3&&hour1==2)
	{
		hour0=0;
		hour1=0;
	}
	else
	{
		hour0++;


	}

}
void inchour1(){


		hour1++;



}

void display(){

	PORTA &= (PORTA & ~0x3F);
	PORTA |= (1<<5);
	PORTC = (PORTC & 0xF0) | (sec0& 0x0F);
	_delay_ms(5);

	PORTA &= (PORTA & ~0x3F);
	PORTA |= (1<<4);
	PORTC = (PORTC & 0xF0) | (sec1 & 0x0F);
	_delay_ms(5);

	PORTA &= ~(PORTA & 0x3F);
	PORTA |=(1<<3);
	PORTC = (PORTC & 0xF0) | (min0 & 0x0F);
	_delay_ms(5);

	PORTA &= (PORTA & ~0x3F);
	PORTA |=(1<<2);
	PORTC = (PORTC & 0xF0) | (min1 & 0x0F);
	_delay_ms(5);

	PORTA &= (PORTA & ~0x3F);
	PORTA |=(1<<1);
	PORTC = (PORTC & 0xF0) | (hour0 & 0x0F);
	_delay_ms(5);

	PORTA &= (PORTA & ~0x3F);
	PORTA |=(1<<0);
	PORTC = (PORTC & 0xF0) | (hour1 & 0x0F);
	_delay_ms(5);


}
int main(void)
{
	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC_Init();


	DDRC  |= 0x0F;
	DDRA |=0xFF;
	clock_init();

	while(1)
		{
		display();

			if (TIME1_Flag==1)
			{
				incsec0();

				TIME1_Flag=0;

			}

			if(INT0_Flag == 1)
				{
					reset();

					INT0_Flag = 0; // reset the interrupt flag
				}
			if(INT1_Flag == 1)
				{

					pause_timer1();
					INT1_Flag = 0; // reset the interrupt flag
				}
			if(INT2_Flag == 1)
					{

					resume_time1_CTC();
						INT2_Flag = 0; // reset the interrupt flag
					}
		}

}



