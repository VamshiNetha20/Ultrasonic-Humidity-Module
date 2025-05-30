#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PD2
#define MOSFET_PIN PD3
#define LED_PIN    PD4
#define BAUD       9600
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

volatile uint8_t humidifier_state = 0;

void init_io(void);
void init_bluetooth(void);
void bt_send(uint8_t data);
void toggle_humidifier(void);

void init_io(void) {
    DDRD &= ~(1 << BUTTON_PIN);
    PORTD |= (1 << BUTTON_PIN);
    DDRD |= (1 << MOSFET_PIN) | (1 << LED_PIN);
    PORTD &= ~((1 << MOSFET_PIN) | (1 << LED_PIN));
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
}

void init_bluetooth(void) {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    sei();
}

void bt_send(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void toggle_humidifier(void) {
    humidifier_state ^= 1;
    if (humidifier_state) {
        PORTD |= (1 << MOSFET_PIN) | (1 << LED_PIN);
    } else {
        PORTD &= ~((1 << MOSFET_PIN) | (1 << LED_PIN));
    }
    bt_send(humidifier_state ? '1' : '0');
}

ISR(INT0_vect) {
    _delay_ms(10);
    if (!(PIND & (1 << BUTTON_PIN))) {
        toggle_humidifier();
        while (!(PIND & (1 << BUTTON_PIN)));
    }
}

ISR(USART_RX_vect) {
    uint8_t cmd = UDR0;
    if (cmd == '1') {
        humidifier_state = 1;
        PORTD |= (1 << MOSFET_PIN) | (1 << LED_PIN);
        bt_send('1');
    } else if (cmd == '0') {
        humidifier_state = 0;
        PORTD &= ~((1 << MOSFET_PIN) | (1 << LED_PIN));
        bt_send('0');
    }
}

int main(void) {
    init_io();
    init_bluetooth();
    while (1) {}
    return 0;
}