#include "lcd-driver.h"

#include "fonts.h"
#include "macros.h"
#include "hw-iface.h"

#ifdef M_PC
#include <stdio.h>
#endif
#ifdef M_AVR
#include <util/delay.h>
#endif

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
static void lcd_display_turn_off (uint8_t aOff) { hw_write_cmd (0xAEU | (0x01U & aOff)); }
/**
 * Set the start line of the LCD module [0..31]
 * @param[in] aStartLine The start line to be set
 */
static void lcd_set_start_line (uint8_t aStartLine)
{ hw_write_cmd (0xC0U | (aStartLine & 0x1FU)); }
/**
 * Set the current page [0..3]
 * @param[in] aPage The page to be set
 */
static void lcd_set_page (uint8_t aPage) { hw_write_cmd (0xACU | (aPage & 0x03U)); }
/**
 * Set the current address
 * @param[in] aAddress The address to be set
 */
static void lcd_set_address (uint8_t aAddress) { hw_write_cmd (aAddress & 0x7FU); }
/**
 * Set ADC flag
 * @param[in] aAdc 0 - forward scan, 1 - backward scan
 */
static void lcd_select_adc (uint8_t aAdc) { hw_write_cmd (0xA0U | (aAdc & 0x01U)); }
/**
 * Turn ON/OFF static (power-saving) mode
 * @param[in] aStatic TRUE - turn ON static mode, FALSE - turn OFF static mode
 */
static void lcd_set_static_mode (uint8_t aStatic) { hw_write_cmd (0xA4U | (aStatic & 0x01U)); }
/**
 * Select duty for the LCD module
 * @param[in] aDuty Should be used '0'
 */
static void lcd_select_duty (uint8_t aDuty) { hw_write_cmd (0xA8U | (aDuty & 0x01U)); }
/**
 * Start read-modify mode
 */
static void lcd_read_modify_start (void) { hw_write_cmd (0xE0U); }
/**
 * End read-modify mode
 */
static void lcd_read_modify_end (void) { hw_write_cmd (0xEEU); }
/**
 * Reset the LCD module
 */
static inline void lcd_reset (void) { hw_write_cmd (0xE2U); }


#define M_LCD_WIDTH (61)
#define M_LCD_HEIGHT (16)
#define M_LCD_RAM (M_LCD_WIDTH*M_LCD_HEIGHT/8)
#define M_LCD_BANK_LENGTH M_LCD_WIDTH

/**
 * These buffers correspond to the real RAM buffers in the LCD module
 */
static uint8_t TheDisplayBank1[M_LCD_BANK_LENGTH];
static uint8_t TheDisplayBank2[M_LCD_BANK_LENGTH];

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

    lcd_reset ();
    lcd_select_adc (0);
    lcd_read_modify_start ();
    lcd_read_modify_end ();

    lcd_set_start_line (0);
    lcd_set_page (0);
    lcd_set_address (0);
}

void lcd_deinit (void) {
}

void lcd_flash (void) {
    uint8_t i;

    /* send our local buffers to the LCD module, the first buffer first */
    lcd_set_page (0);
    lcd_set_address (0);
    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        lcd_write_byte (FALSE, TheDisplayBank1[i]);
    }

    /* and the other one the second */
    lcd_set_page (1);
    lcd_set_address (0);
    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        lcd_write_byte (FALSE, TheDisplayBank2[i]);
    }
}

void lcd_clear (void) {
    uint8_t i;

    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        TheDisplayBank1[i] = 0;
        TheDisplayBank2[i] = 0;
    }
}

void lcd_debug_show_buffer (void) {
    uint8_t y;
    uint16_t x;
    uint8_t bank;

    for (bank = 0; bank < 2; bank++) {
        for (y = 0; y < 8; y++) {
            uint8_t bit_mask;

            bit_mask = (1 << y);
            printf ("[");
            for (x = 0; x < 61; x++) {
                if (0 == bank) {
                    if (bit_mask & TheDisplayBank1[60 - x]) {
                        printf ("XX");
                    }
                    else {
                        printf ("  ");
                    }
                }
                else {
                    if (bit_mask & TheDisplayBank2[60 - x]) {
                        printf ("XX");
                    }
                    else {
                        printf ("  ");
                    }
                }
            }
            printf ("]\n");
        }
    }
}

void lcd_print_char (uint8_t aX, uint8_t aY, uint8_t aChar) {
    uint8_t x;
    uint8_t y;

    for (y = 0; y < 14; y++) {
        mubyte image;

        image = get_font14_byte (aChar, y);
        for (x = 0; x < 8; x++) {
            uint8_t mask;

            mask = (1 << x);
            lcd_set_pixel (aX + 7 - x, y + aY, mask & image);
        }
    }
}

void lcd_paint_char (uint8_t aX, uint8_t aY, uint8_t aChar) {
    uint8_t x;
    uint8_t y;

    for (y = 0; y < 14; y++) {
        mubyte image;

        image = get_font14_byte (aChar, y);
        for (x = 0; x < 8; x++) {
            if ((1 << x) & image) {
                lcd_set_pixel (aX + 7 - x, y + aY, TRUE);
            }
        }
    }
}

void lcd_set_pixel (uint8_t aX, uint8_t aY, uint8_t aValue) {
    uint8_t bit_mask;

    m_return_if_fail (aX < 61);
    m_return_if_fail (aY < 16);

    aX = 60 - aX;

    bit_mask = (1 << (aY & 0x07U));
    if (aY & 0x08U) {
        /* 2-nd bank */
        if (aValue) {
            TheDisplayBank2[aX] = TheDisplayBank2[aX] | bit_mask;
        }
        else {
            TheDisplayBank2[aX] = TheDisplayBank2[aX] & ~bit_mask;
        }
    }
    else {
        /* 1-st bank */
        if (aValue) {
            TheDisplayBank1[aX] = TheDisplayBank1[aX] | bit_mask;
        }
        else {
            TheDisplayBank1[aX] = TheDisplayBank1[aX] & ~bit_mask;
        }
    }
}

static void lcd_write_byte (uint8_t aCommand, uint8_t aByte) {
    M_UNUSED_PARAM (aByte);

    /* wait until the LCD module is ready */
    while (0x80U & lcd_read_status ()) {}

    if (aCommand) {
        printf ("Writing a command byte: 0x%2.2x\n", aByte);
    }
    else {
        printf ("Writing a data byte: 0x%2.2x\n", aByte);
    }
}

static uint8_t lcd_read_byte (uint8_t aCommand) {
    if (aCommand) {
        return 0x40;
    }
    else {
        return 0x00;
    }
}

static void lcd_set_reset_pin (uint8_t aOn) {
    M_UNUSED_PARAM (aOn);

    printf ("Changing RESET pin: [%d]\n", aOn);
}
