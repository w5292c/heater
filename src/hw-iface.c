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
 * -----------------------------------------------------------------------------
 * |      A0 |   O | PC7 | TOSC2 (Timer Oscillator Pin 2)                      |
 * |   RD/WR |   O | PC6 | TOSC1 (Timer Oscillator Pin 1)                      |
 * |       E |   O | PB1 | T1 (Timer/Counter1 External Counter Input)          |
 * |     RES |   O | PB0 | T0 (Timer/Counter0 External Counter Input)          |
 * -----------------------------------------------------------------------------
 */

/**
 * Read LCD module
 * @param[in] aStatus FALSE - read a data byte, TRUE - read a status byte
 * @return The read byte
 */
static muint8 hw_read_lcd (muint8 aStatus);
/**
 * Write LCD module
 * @param[in] aCmd FALSE - write a data byte, TRUE - write a status byte
 * @param[in] aByte The data to be written
 */
static void hw_write_lcd (muint8 aCmd, muint8 aByte);

void hw_init (void) {
#ifdef M_AVR
    /* Define PA0..PA3 and PD4..PD7 pins as inputs for now */
    PORTA = 0x00U;
    DDRA &= ~((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
    PORTD = 0x00U;
    DDRD &= ~((1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
    /* Define PB0, PB1, PC6 and PC7 pins as outputs */
    PORTB = 0x00U;
    DDRB |= (1<<DDB0) | (1<<DDB1);
    PORTC = 0x00U;
    DDRC |= (1<<DDC6) | (1<<DDC7);
#endif /* M_AVR */

#ifdef M_PC
    printf ("Initializing the HW interface\n");
#endif /* M_PC */
}

muint8 hw_read_data (void) {
    return hw_read_lcd (FALSE);
}

muint8 hw_read_status (void) {
    return hw_read_lcd (TRUE);
}

static void hw_write_lcd (muint8 aCmd, muint8 aByte) {
#ifdef M_AVR
    /* make sure the LCD module is ready to receive commands */
    while (0x80U & hw_read_status ()) {}

    /* pull 'E' (PB1) pin low (LCD module not selected) */
    PORTB &= ~(1<<PB1);
    if (aCmd) {
        /* pull A0 (PC7) pin low (choose 'command' mode) */
        PORTC &= ~(1<<PC7);
    }
    else {
        /* pull A0 (PC7) pin high (choose 'data' mode) */
        PORTC |= (1<<PC7);
    }
    /* pull RD/WR (PC6) pin low (choose 'write' mode) */
    PORTC &= ~(1<<PC6);
    /* a bit of a delay to set address */
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    /* pull 'E' (PB1) pin high (select LCD module) */
    PORTB |= (1<<PB1);
    /* make the data pins output */
    DDRA |= ((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
    DDRD |= ((1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
    /* send the data byte out */
    PORTA = (aByte & 0x0FU);
    PORTD = (aByte & 0xF0U);
    /* Sync + 400 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    /* pull 'E' (PB1) pin low again (LCD module not selected) */
    PORTB &= ~(1<<PB1);
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    /* make the data port input again */
    DDRA &= ~((1<<DDA0) | (1<<DDA1) | (1<<DDA2) | (1<<DDA3));
    DDRD &= ~((1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));
    PORTA = 0x00U;
    PORTD = 0x00U;
    /* Sync + 400 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
#endif /* M_AVR */

#ifdef M_PC
    if (aCmd) {
        printf ("Writing command: [0x%2.2x]\n", aByte);
    }
    else {
        printf ("Writing data: [0x%2.2x]\n", aByte);
    }
#endif /* M_PC */
}

static muint8 hw_read_lcd (muint8 aStatus) {
    muint8 status;

#ifdef M_AVR
    /* pull 'E' (PB1) pin low (LCD module not selected) */
    PORTB &= ~(1<<PB1);
    if (aStatus) {
        /* pull A0 (PC7) pin low (choose 'command' mode) */
        PORTC &= ~(1<<PC7);
    }
    else {
        /* pull A0 (PC7) pin high (choose 'data' mode) */
        PORTC |= (1<<PC7);
    }
    /* pull RD/WR (PC6) pin high (choose 'read' mode) */
    PORTC |= (1<<PC6);
    /* a bit of a delay to set address */
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    /* pull 'E' (PB1) pin high (select LCD module) */
    PORTB |= (1<<PB1);
    /* Sync + 400 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    /* read the status now */
    status = (PINA&0x0FU)|(PIND&0xF0U);
    /* pull 'E' (PB1) pin low again (LCD module not selected) */
    PORTB &= ~(1<<PB1);
    /* Sync + 400 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
#endif /* M_AVR */

#ifdef M_PC
    if (aStatus) {
        printf ("Reading status\n");
        status = 0x40U;
    }
    else {
        printf ("Reading data\n");
        status = 0x00U;
    }
#endif /* M_PC */
    return status;
}

void hw_write_cmd (muint8 aCmd) {
    hw_write_lcd (TRUE, aCmd);
}

void hw_write_data (muint8 aData) {
    hw_write_lcd (FALSE, aData);
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
