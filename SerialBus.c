#define F_CPU 			16000000

#include <avr/io.h>
#include <util/delay.h>

#define OUT_PORT	PORTB
#define CLOCK 		PB5		// PB5: Clock (white)
#define	DATA 		PB6		// PB6: Data (green)
#define	RESET 		PB7		// PB7:	Reset (orange)

inline uint8_t readBit(volatile uint8_t port, uint8_t pin) {
	return ((port >> pin) & 1);
}

inline void writeBit(volatile uint8_t *port, uint8_t pin, uint8_t value) {
	if(!value) {
		*port &= ~(1 << pin);
	} else {
		*port |= (1 << pin);
	}
}

void init() {
	DDRB = 0xff;
	PORTB = 0x00;

	DDRD = 0x00;
	DDRC = 0x00;

	PORTD = 0x00;
	PORTC = 0x00;
}

uint8_t readToggleInputs() {
	uint8_t result = 0;

	writeBit(&result, 0, readBit(PINC, PC2));
	writeBit(&result, 3, readBit(PIND, PD0));
	writeBit(&result, 2, readBit(PIND, PD1));
	writeBit(&result, 1, readBit(PIND, PD2));

	return result;
}

uint8_t readIncrementInputs() {
	uint8_t result = 0;

	writeBit(&result, 3, readBit(PIND, PD3));
	writeBit(&result, 2, readBit(PIND, PD4));
	writeBit(&result, 1, readBit(PIND, PD5));
	writeBit(&result, 0, readBit(PIND, PD6));

	return result;
}

void resetShift() {
	writeBit(&OUT_PORT, RESET, 1);
	writeBit(&OUT_PORT, RESET, 0);
}

void shiftIn(uint8_t data, uint8_t number) {
	uint8_t i;

	for(i = 0; i < number; i++) {
		writeBit(&OUT_PORT, DATA, readBit(data, i));

		writeBit(&OUT_PORT, CLOCK, 1);
		writeBit(&OUT_PORT, CLOCK, 0);
	}
}

// This works, stop fucking about with it
void debugOut(uint8_t value) {
	uint8_t mask = 0b11110000;

	value <<= 4;

	PORTB = (PORTB & ~mask) | (value & mask);
}

int main(void) {
	init();

	shiftIn(0xff, 8);
	_delay_ms(100);
	resetShift();
	_delay_ms(50);

	while(1) {
		resetShift();

		shiftIn(readToggleInputs(), 4);

		shiftIn(readIncrementInputs(), 4);

		_delay_ms(2);
	}
}
