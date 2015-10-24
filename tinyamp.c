//dont change anything, this is a working example
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/*
           ATTINY 45 pinout                                                     
                                                                                  
                  +----+                                                          
  ADC0/_RST/ PB5 -|1  8|-VCC                                                      
  ADC3/XTAl1 PB3 -|2  7|-PB2         SCK                                             
    OCR1B    PB4 -|3  6|-PB1  OCR1A  MISO                                             
             GND -|4  5|-PB0         MOSI                                             
                  +----+                                                          
							                               
*/

int main(void)
{
	//ports 
	DDRB = (1<<DDB0)|(1 << DDB1)|(1<<DDB4);//PB1 & PB4 = pin6 & pin 3

	//ADC
	//internal 2.56 no cap, ADC3 = pin2 = pb3
	ADMUX = (1<<REFS2)|(1<<REFS1)|(1<<MUX1)|(1<<MUX0);
	//ADMUX = 0b00000011;
	//div factor 16	
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2);

	PRR =  0b00000110; 
	MCUCR =0b00100000; 

	//PWM
	//pwm, noninv, clk/2
	TCCR1 = (1<<PWM1A)|(1<<COM1A1)|(1<<CS11)|(1<<CS10);
	//TCCR1 = (1<<PWM1A)|(1<<COM1A1)|(1<<CS11);
	GTCCR = (1<<PWM1B)|(1<<COM1B1);
	TIMSK = 1<<TOIE1;
	//asynchronous clock + PLL enable
	PLLCSR = (1<<PCKE)|(1<<PLLE);

	sei();
 
	for(;;){
		sleep_cpu();
	}

	return 0;
}

uint8_t phase2P, phase2N,phase1P, phase1N;
char phase = 0;
uint16_t adc = 255;
uint16_t temp = 0;

ISR(TIMER1_OVF_vect)
{
	//static char phase = 0;
	//static uint16_t adc=0;

	if (1 & ++phase){
		//read only half the period cos
		//we use two periods per sample
		//8 bits x 2 = 9bits
		temp = (ADC>>1);
			
		if ((temp<511))
			adc = temp;

		if ((adc & 0b100000000) == 0b100000000){
		//if (adc>255){
			phase2P = 0xFF;
			phase2N = 0x00;
			//phase1P = (uint8_t)(0b0000000011111111&adc);
			phase1P = (uint8_t)(adc-255);
			phase1N = 0xFF-phase1P;
		}else{
			phase2P = 0x00;
			phase2N = 0xFF;
			phase1P = (uint8_t)adc;
			phase1N = 0xFF-phase1P;
		}
		OCR1A = phase1P;
	        OCR1B = phase1N;

	} else{

		OCR1A = phase2P;
		OCR1B = phase2N;
	}

	//PORTB ^= 0b1;	

}
