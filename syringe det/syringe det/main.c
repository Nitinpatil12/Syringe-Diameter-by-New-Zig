#define F_CPU                           (4000000UL)         /* using default clock 4MHz*/
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "UART_1_AVR128DA64.h"
#define channel_3 0x03
//#define channel_4 0x04
#define START_TOKEN 0x03
#define END_TOKEN 0xFC

void ADC0_init(void)
{
	VREF.ADC0REF = 0x5 | (1 << 7);
	ADC0.CTRLC = ADC_PRESC_DIV4_gc;        // CLK_PER divided by 4
	ADC0.CTRLA = ADC_ENABLE_bm;
	PORTD.IN  = ADC_RESSEL_12BIT_gc;
}
unsigned long size=0, SYRINGE_Value=0;

void ADC0_start(void)
{
	
	ADC0.COMMAND = ADC_STCONV_bm;
}

float ADC0_read(int pin)
{
	
	ADC0.MUXPOS = pin;
	ADC0_start();
	while (!(ADC0.INTFLAGS & ADC_RESRDY_bm));
	return ADC0.RES;
}
int main(void)
{
	float syringe_dia = 0.0;
	float sum;
	USART1_init(9600);
	ADC0_init();
	int adc ;
	PORTC.DIRCLR = PIN7_bm;
	PORTC.PIN7CTRL |= PORT_PULLUPEN_bm;
	while (1)
	{
		//adc = ADC0_read( channel_3);
		sum = 0.00;
		for(int i=1;i<=500;i++)
		{
			adc= ADC0_read(channel_3);
			sum=sum+adc;
		}
		
		sum=sum/500.00;
		syringe_dia =  0.00676255*(sum) + 5.377882;
		USART1_sendFloat(syringe_dia, 2);
		_delay_ms(100);
	}
	
}

