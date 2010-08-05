#include "lcd-driver.h"

#include "macros.h"

#include <stdio.h>
#include <util/delay.h>

/**
 * Set RESET pin of the LCD module
 * @param[in] aOn The value to be set
 */
static void lcd_set_reset_pin (uint8_t aOn);
/**
 * Low-level request to write byte to the LCD module
 * @param[in] aCommand TRUE - write a command byte; FALSE - write a data byte
 * @param[in] aByte The byte to be written to the LCD module
 */
static void lcd_write_byte (uint8_t aCommand, uint8_t aByte);

/**
 * Low-level request to read byte in the LCD module
 * @param[in] aCommand TRUE - read a command byte; FALSE - read a data byte
 * @return The read byte
 */
static uint8_t lcd_read_byte (uint8_t aCommand);

/**
 * Read the status of the LCD module
 * @return The status byte of the LCD module
 */
static inline uint8_t lcd_read_status () { return lcd_read_byte (TRUE); }

/**
 * Turn ON/OFF display
 * @param[in] aOn FALSE - Turn ON; TRUE - Turn OFF
 */
static void lcd_display_turn_off (uint8_t aOn);
/**
 * Set the start line of the LCD module [0..31]
 * @param[in] aStartLine The start line to be set
 */
static void lcd_set_start_line (uint8_t aStartLine);
/**
 * Set the current page [0..3]
 * @param[in] aPage The page to be set
 */
static void lcd_set_page (uint8_t aPage);
/**
 * Set the current address
 * @param[in] aAddress The address to be set
 */
static void lcd_set_address (uint8_t aAddress);
/**
 * Set ADC flag
 * @param[in] aAdc 0 - forward scan, 1 - backward scan
 */
static void lcd_select_adc (uint8_t aAdc);
/**
 * Turn ON/OFF static (power-saving) mode
 * @param[in] aStatic TRUE - turn ON static mode, FALSE - turn OFF static mode
 */
static void lcd_set_static_mode (uint8_t aStatic);
/**
 * Select duty for the LCD module
 * @param[in] aDuty Should be used '0'
 */
static void lcd_select_duty (uint8_t aDuty);
/**
 * Start read-modify mode
 */
static void lcd_read_modify_start (void);
/**
 * End read-modify mode
 */
static void lcd_read_modify_end (void);
/**
 * Reset the LCD module
 */
static void lcd_reset (void);

/**
 * Internal buffer for a 61X16 dot display
 * The buffer contains butmap in the following format:
 *     Raw:    00    01    02    03    04    05     X      X     X    60
 * Line 00: <000> <001> <002> <003> <004> <005> <xxx>  <xxx> <xxx> <060>
 * Line 01: <060> <061> <062> <063> <064> <065> <xxx>  <xxx> <xxx> <060>
 * Line 02:
 * Line 03:
 * X X X
 * Line 15:
 */
static uint8_t TheDisplayBuffer[122];

void lcd_init (void) {
    /* после подачи напряжения питания удерживать вывод RES
       в состоянии логического “0” еще не менее 10 мкс */
    lcd_set_reset_pin (FALSE);
    _delay_us(50);

    /* подать перепад на вывод RES c логического “0” в логическую “1”,
       длительность фронта не более 10 мкс */
    lcd_set_reset_pin (TRUE);

    /* ожидать сброса бита RESET в байте состояния или выждать не менее 2 мс */
    _delay_ms(2);
    while (0x10U & lcd_read_status ()) {}

    /* подать команду снятия флага RMW (END) */
    lcd_read_modify_end ();

    /* подать команду включения обычного режима работы (Static Drive ON/OFF) */
    lcd_set_static_mode (FALSE);

    /* подать команду выбора мультиплекса (Duty Select) */
    lcd_select_duty (0);

    /* подать команду включения дисплея (Display ON/OFF) */
    lcd_display_turn_off (FALSE);

    lcd_set_start_line (0);
    lcd_set_page (0);
    lcd_set_address (0);
    lcd_select_adc (0);
    lcd_read_modify_start ();
    lcd_reset ();

    M_UNUSED_PARAM (TheDisplayBuffer[0]);
}

void lcd_deinit (void) {
}

void lcd_flash (void) {
}

void lcd_clear (void) {
}

void lcd_print_char (uint8_t aX, uint8_t aY, uint8_t aChar) {
    printf ("Printing: [%c] at (%d, %d)", aChar, aX, aY);
}

static void lcd_display_turn_off (uint8_t aOff) {
    lcd_write_byte (TRUE, 0xAEU | (0x01U & aOff));
}

static void lcd_set_start_line (uint8_t aStartLine) {
    lcd_write_byte (TRUE, 0xC0U | (aStartLine & 0x1FU));
}

static void lcd_set_page (uint8_t aPage) {
    lcd_write_byte (TRUE, 0xACU | (aPage & 0x03U));
}

static void lcd_set_address (uint8_t aAddress) {
    lcd_write_byte (TRUE, aAddress & 0x7FU);
}

static void lcd_select_adc (uint8_t aAdc) {
    lcd_write_byte (TRUE, 0xA0U | (aAdc & 0x01U));
}

static void lcd_set_static_mode (uint8_t aStatic) {
    lcd_write_byte (TRUE, 0xA4U | (aStatic & 0x01U));
}

static void lcd_select_duty (uint8_t aDuty) {
    lcd_write_byte (TRUE, 0xA8U | (aDuty & 0x01U));
}

static void lcd_read_modify_start (void) {
    lcd_write_byte (TRUE, 0xE0U);
}

static void lcd_read_modify_end (void) {
    lcd_write_byte (TRUE, 0xEEU);
}

static void lcd_reset (void) {
    lcd_write_byte (TRUE, 0xE2U);
}

static void lcd_write_byte (uint8_t aCommand, uint8_t aByte) {
    /* wait until the LCD module is ready */
    while (0x80U & lcd_read_status ()) {}

    if (aCommand) {
        printf ("Writing a command byte: 0x%x\n", aByte);
    }
    else {
        printf ("Writing a data byte: 0x%x\n", aByte);
    }
}

static uint8_t lcd_read_byte (uint8_t aCommand) {
    if (aCommand) {
        return 0x50;
    }
    else {
        return 0x00;
    }
}

static void lcd_set_reset_pin (uint8_t aOn) {
    printf ("Changing RESET pin: [%d]\n", aOn);
}
