#include "hw-iface.h"

#include "macros.h"

#ifdef M_PC
#include <stdio.h>
#endif

#ifdef M_AVR
#include <avr/io.h>
#include <util/delay.h>
#endif /* M_AVR */

/**
 * Connection diagramm:
 * -----------------------------------------------------------------------------
 * | LCD pin | I/O |  MC |                                                     |
 * -----------------------------------------------------------------------------
 * |     DB7 | I/O | PD7 | OC2 (Timer/Counter2 Output Compare Match Output)    |
 * |     DB6 | I/O | PD6 | ICP1 (Timer/Counter1 Input Capture Pin)             |
 * |     DB5 | I/O | PD5 | OC1A (Timer/Counter1 Output Compare A Match Output) |
 * |     DB4 | I/O | PD4 | OC1B (Timer/Counter1 Output Compare B Match Output) |
 * |     DB3 | I/O | PA3 | ADC3 (ADC input channel 3)                          |
 * |     DB2 | I/O | PA2 | ADC2 (ADC input channel 2)                          |
 * |     DB1 | I/O | PA1 | ADC1 (ADC input channel 1)                          |
 * |     DB0 | I/O | PA0 | ADC0 (ADC input channel 0)                          |
 * |         |     |     |                                                     |
 * |      A0 |   O | PC7 | TOSC2 (Timer Oscillator Pin 2)                      |
 * |   RD/WR |   O | PC6 | TOSC1 (Timer Oscillator Pin 1)                      |
 * |       E |   O | PB1 | T1 (Timer/Counter1 External Counter Input)          |
 * |     RES |   O | PB0 | T0 (Timer/Counter0 External Counter Input)          |
 * -----------------------------------------------------------------------------
 */

void hw_init (void) {
#ifdef M_AVR
    /* Define PA0..PA3 and PD4..PD7 pins as inputs for now */
    PORTA = 0x00U;
    DDRA &= ~((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
    PORTD = 0x00U;
    DDRD &= ~((1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
    /* Define PB0, PB1, PC6 and PC7 pins as outputs */
    PORTB = 0x01U;
    DDRB |= (1<<DDB0) | (1<<DDB1);
    PORTC = 0x80U;
    DDRC |= (1<<DDC6) | (1<<DDC6);
#endif /* M_AVR */
}

uint8_t hw_read_data (void) {
    printf ("Reading data\n");
    return 0x00;
}

uint8_t hw_read_status (void) {
    printf ("Reading status\n");
    return 0x40;
}

void hw_write_cmd (uint8_t aCmd) {
    M_UNUSED_PARAM (aCmd);

    /* wait until the LCD module is ready */
    while (0x80U & hw_read_status ()) {}

    /* */
    printf ("Writing command: [%2.2x]\n", aCmd);
}

void hw_write_data (uint8_t aData) {
    M_UNUSED_PARAM (aData);

    printf ("Writing data: [%2.2x]\n", aData);
}

void hw_reset_lcd (void) {
#ifdef M_AVR
    /* set the 'RESET' pin to low */
    PORTB &= ~(1<<PB0);
    _delay_us(50);
    /* set the 'RESET' pin to low */
    PORTB |= (1<<PB0);
    while (0x10U & hw_read_status ()) {}
#endif /* M_AVR */

#ifdef M_PC
    printf ("Resetting LCD module\n");
#endif /* M_PC */
}
