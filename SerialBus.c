#define F_CPU 			16000000

#include <avr/io.h>
#include <util/delay.h>

void init() {
	DDRB = 0xff;
	PORTB = 0x00;
}

int main(void) {
	init();

	while(1) {
		PORTB++;
		_delay_ms(10);
	}
}
