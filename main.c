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

void usart_print_char(unsigned char c) {
	transmitter_wait();
	UDR0 = c;
}

void usart_print(const char* str) {
	for(uint16_t i = 0; str[i] != '\0'; i++) {
		usart_print_char(str[i]);
	}
}

void usart_println(const char* str) {
	usart_print(str);
	usart_print_char('\r'); // Goto start of line
	usart_print_char('\n');
}

int main() {
	usart_init();
	usart_println("Hello, world!");
	usart_print("Hello, world!");
	while(1) {
	}
	return 0;
}
