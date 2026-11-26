#include <avr/io.h>
#include <stdbool.h>

#define BAUD_RATE 9600

void usart_init() {
	// set asynchronous mode
	UCSR0C  &= ~((1 << UMSEL00) | (1 << UMSEL01));

	// set baud rate
	uint16_t ubrr = F_CPU / (16UL * BAUD_RATE) - 1;
	UBRR0H = (uint8_t) (ubrr >> 8);
	UBRR0L = (uint8_t) ubrr;

	// configure as receiver and transmitter
	UCSR0B  |= (1 << RXEN0) | (1 << TXEN0);

	// set 2 stop bits
	UCSR0C  |= (1 << USBS0);

	// set 8 data bits
	UCSR0C  |= (1 << UCSZ00) | (1 << UCSZ01);
	UCSR0B  &= ~(1 << UCSZ02);

	// disable parity bits
	UCSR0C  &= ~((1 << UPM00) | (1 << UPM01));
}

bool transmitter_ready() {
	return (UCSR0A & (1 << UDRE0)) == (1 << UDRE0);
}

void transmitter_wait() {
	while(!transmitter_ready()) {
		// Busy-wait untill transmitter is ready
	}
}

void usart_print(unsigned char c) {
	transmitter_wait();
	UDR0 = c;
}

int main() {
	usart_init();
	while(1) {
		usart_print('a');
	}
	return 0;
}
