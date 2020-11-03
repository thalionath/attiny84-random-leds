
#define	F_CPU 8000000UL

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>

void adc_on()
{
	/*
	ADC Prescaler needs to be set so that the ADC input frequency is between 50 - 200kHz.
	
	Example prescaler values for various frequencies
	
	Clock   Available prescaler values
   ---------------------------------------
	 1 MHz   8 (125kHz), 16 (62.5kHz)
	 4 MHz   32 (125kHz), 64 (62.5kHz)
	 8 MHz   64 (125kHz), 128 (62.5kHz)
	16 MHz   128 (125kHz)
	*/

   ADMUX =
            (0 << REFS1) |     // Sets ref. voltage to Vcc, bit 1   
            (0 << REFS0) |     // Sets ref. voltage to Vcc, bit 0
            (0 << MUX5)  |     // use ADC1 for input (PA1), MUX bit 5
            (0 << MUX4)  |     // use ADC1 for input (PA1), MUX bit 4
            (0 << MUX3)  |     // use ADC1 for input (PA1), MUX bit 3
            (0 << MUX2)  |     // use ADC1 for input (PA1), MUX bit 2
            (0 << MUX1)  |     // use ADC1 for input (PA1), MUX bit 1
            (1 << MUX0);       // use ADC1 for input (PA1), MUX bit 0

  ADCSRA = 
            (1 << ADEN)  |     // Enable ADC 
            (1 << ADPS2) |     // set prescaler to 64
            (1 << ADPS1) |      
            (0 << ADPS0);      
			
  ADCSRB = 
            (1 << ADLAR);      // left shift result (for 8-bit values)
	//        (0 << ADLAR);      // right shift result (for 10-bit values)
}

// https://www.heise.de/ct/artikel/Erste-Schritte-mit-den-Mikrocontrollern-ATtiny84-und-85-4399393.html
// http://www.marcelpost.com/wiki/index.php/ATtiny84_ADC

// Generates 3 random bits ands sets outputs PB0:2 accordingly
// and goes to power down after 5 seconds.
int main(void)
{
	DDRA = 0b00000000;
	DDRB = 0b00000111; // outputs
	
#if 1
	adc_on();
	
	uint8_t random = 0;
	
	// we want at least one bit set
	while((random & 7) == 0)
	{
		// get 3 random bits (LSB of an ADC sample)
		for(int i=0; i<3; ++i) {
			random <<= 1;
				
			ADCSRA |= (1 << ADSC);         // start ADC measurement
			while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
				
			random |= (ADCH & 1);
		}
	}
	
	PORTB = (random & 7);
#else
	PORTB = 7:
#endif

	// ADC OFF
	ADMUX  = 0;
	ADCSRA = 0;
	
	_delay_ms(5000);

	DDRB = 0;

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sleep_mode(); // system sleeps here

    while (1) 
    {
    }
}

