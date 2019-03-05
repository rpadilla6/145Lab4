#include "avr.h"
#include <stdio.h>
#include <string.h>
#include "lcd.h"

void
avr_init(void)
{
	WDTCR = 15;
}

void
avr_wait(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}
char str[17];
char out[17];

void init_adc(){
	SET_BIT(ADMUX, REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(0<<ADPS0);
	SET_BIT(ADMUX, ADLAR);
	ADMUX&=0xF0;
}

uint16_t read_adc(uint8_t ch){
	ADMUX|=0;
	SET_BIT(ADCSRA, ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	SET_BIT(ADCSRA, ADIF);
	return(ADC);
}

int main(void){
	// Initialize everything
	avr_init();	
	lcd_init();
	init_adc();
	for(;;){
		//Main loop will check if key pressed, and if it is A or B, do something
		uint16_t value = read_adc(0);
		avr_wait(500);
		lcd_clr();
		lcd_pos(1,1);
		sprintf(str, "%d", value);
		lcd_puts2(str);
	}
}

/************************************************************************/
/* Gets actual keypad value (numbers 0-9)                               */
/************************************************************************/
int get_num(void){
	int num = 0;
	for(;;){
		int key = get_key();
		switch(key){
			// # is enter key
			case 15:
				return num;
			case 0:
			case 4:
			case 8:
			case 12: 
			case 13:
			case 16: // do nothing in this case
				break;
			default:
				key = key - ((key-1)/4);
				if(key == 11) key = 0;
				num = (num * 10) + key;
				sprintf(out, "%d", key);
				lcd_puts2(out);
				avr_wait(200);
				break;
		}
	}
}



/************************************************************************/
/* Check for if a certain button is pressed                             */
/************************************************************************/
int is_pressed(int row, int col){
	//set all rows, cols to n/c
	DDRC=0;
	PORTC=0;
	//set col to strong 0
	SET_BIT(DDRC, col+4);
	//set row to weak 1
	SET_BIT(PORTC, row);
	avr_wait(1);
	return !GET_BIT(PINC, row);
}

/************************************************************************/
/* Get raw key pressed, different than get_num which does some conversion*/
/* for keypad numbers                                                   */
/************************************************************************/
int get_key(){
	int r,c;
	for(r=0;r<4;++r){
		for(c=0;c<4;++c){
			if(is_pressed(r,c)){
				return 1+(r*4)+c;
			}
		}
	}
	return 0;
}