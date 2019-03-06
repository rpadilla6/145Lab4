#ifndef _AVR_H
#define _AVR_H

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdlib.h>

#define XTAL_FRQ 8000000lu

#define SET_BIT(p,i) ((p) |=  (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) &   (1 << (i)))

#define WDR() asm volatile("wdr"::)
#define NOP() asm volatile("nop"::)
#define RST() for(;;);
#define ONEVT 204.6
#define A 1784
struct note{
	float freq;
	int dur;
};

void avr_init(void);

void avr_wait(unsigned short msec);
int get_key();
int get_num();

int is_pressed(int, int);
void blink(int);

#endif /* _AVR_H */