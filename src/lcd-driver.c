#include "lcd-driver.h"

#include "fonts.h"
#include "macros.h"
#include "hw-iface.h"

#ifdef M_PC
#include <stdio.h>
#endif

#ifndef M_PC
#include <avr/pgmspace.h>
#endif

#ifdef M_PC
/**
 * Display the current LCD buffers on console
 */
static void lcd_debug_show_buffer (void);
#endif
/**
 * Turn ON/OFF display
 * @param[in] aOn FALSE - Turn ON; TRUE - Turn OFF
 */
static inline void lcd_display_turn_on (muint8 aOn)
{ hw_write_cmd (0xAEU | (0x01U & aOn)); }
/**
 * Set the start line of the LCD module [0..31]
 * @param[in] aStartLine The start line to be set
 */
static inline void lcd_set_start_line (muint8 aStartLine)
{ hw_write_cmd (0xC0U | (aStartLine & 0x1FU)); }
/**
 * Set the current page [0..3]
 * @param[in] aPage The page to be set
 */
static inline void lcd_set_page (muint8 aPage)
{ hw_write_cmd (0xB8U | (aPage & 0x03U)); }
/**
 * Set the current address
 * @param[in] aAddress The address to be set
 */
static inline void lcd_set_address (muint8 aAddress)
{ hw_write_cmd (aAddress & 0x7FU); }
/**
 * Set ADC flag
 * @param[in] aAdc 0 - forward scan, 1 - backward scan
 */
static inline void lcd_select_adc (muint8 aAdc)
{ hw_write_cmd (0xA0U | (aAdc & 0x01U)); }
/**
 * Turn ON/OFF static (power-saving) mode
 * @param[in] aStatic TRUE - turn ON static mode, FALSE - turn OFF static mode
 */
static inline void lcd_set_static_mode (muint8 aStatic)
{ hw_write_cmd (0xA4U | (aStatic & 0x01U)); }
/**
 * Select duty for the LCD module
 * @param[in] aDuty Should be used '0'
 */
static inline void lcd_select_duty (muint8 aDuty)
{ hw_write_cmd (0xA8U | (aDuty & 0x01U)); }
/**
 * Start read-modify mode
 */
static inline void lcd_read_modify_start (void)
{ hw_write_cmd (0xE0U); }
/**
 * End read-modify mode
 */
static inline void lcd_read_modify_end (void)
{ hw_write_cmd (0xEEU); }
/**
 * Reset the LCD module
 */
static inline void lcd_reset (void)
{ hw_write_cmd (0xE2U); }

#define M_LCD_WIDTH (61)
#define M_LCD_HEIGHT (16)
#define M_LCD_RAM (M_LCD_WIDTH*M_LCD_HEIGHT/8)
#define M_LCD_BANK_LENGTH M_LCD_WIDTH

/**
 * These buffers correspond to the real RAM buffers in the LCD module
 */
static muint8 TheDisplayBank1[M_LCD_BANK_LENGTH];
static muint8 TheDisplayBank2[M_LCD_BANK_LENGTH];

void lcd_init (void) {
    /* initialize the HW pins to the LCD module */
    hw_init ();
    hw_reset_lcd ();

    /* подать команду снятия флага RMW (END) */
    lcd_read_modify_end ();

    /* подать команду включения обычного режима работы (Static Drive ON/OFF) */
    lcd_set_static_mode (FALSE);

    /* подать команду выбора мультиплекса (Duty Select) */
    lcd_select_duty (0);

    /* подать команду включения дисплея (Display ON/OFF) */
    lcd_display_turn_on (TRUE);

    /*lcd_reset ();*/
    lcd_select_adc (0);
    /*lcd_read_modify_start ();*/
    /*lcd_read_modify_end ();*/

    lcd_set_start_line (0);
    lcd_set_page (0);
    lcd_set_address (0);
}

void lcd_deinit (void) {
}

void lcd_flash (void) {
    muint8 i;
    /**
     * The the current screen number
     * Possible values:
     * 0 - The visible lines:  0..15;
     * 1 - The visible lines: 16..31;
     */
    static muint8 current_screen = TRUE;

    /* send our local buffers to the LCD module, the first buffer first */
    lcd_set_page (current_screen ? 2 : 0);
    lcd_set_address (0);
    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        hw_write_data (TheDisplayBank1[i]);
    }

    /* and the other one the second */
    lcd_set_page (current_screen ? 3 : 1);
    lcd_set_address (0);
    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        hw_write_data (TheDisplayBank2[i]);
    }

    /* switch the current screen */
    lcd_set_start_line (current_screen ? 16 : 0);

#ifdef M_PC
    lcd_debug_show_buffer ();
#endif
    /* Prepare current_screen for the next flash request */
    if (current_screen) {
        current_screen = FALSE;
    }
    else {
        current_screen = TRUE;
    }
}

void lcd_clear (void) {
    muint i;

    for (i = 0; i < M_LCD_BANK_LENGTH; i++) {
        TheDisplayBank1[i] = 0;
        TheDisplayBank2[i] = 0;
    }
}

#ifdef M_PC
static void lcd_debug_show_buffer (void) {
    muint y;
    muint x;
    muint bank;

    for (bank = 0; bank < 2; bank++) {
        for (y = 0; y < 8; y++) {
            muint8 bit_mask;

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
#endif

void lcd_print_char (muint8 aX, muint8 aY, muint8 aChar) {
    muint x;
    muint y;

    for (y = 0; y < 14; y++) {
        muint8 image;

        image = get_font14_byte (aChar, y);
        for (x = 0; x < 7; x++) {
            muint8 mask;

            mask = (1 << x);
            lcd_set_pixel (aX + 7 - x, y + aY, mask & image);
        }
    }
}

void lcd_paint_char (muint8 aX, muint8 aY, muint8 aChar) {
    muint x;
    muint y;

    for (y = 0; y < 14; y++) {
        muint8 image;

        image = get_font14_byte (aChar, y);
        for (x = 0; x < 7; x++) {
            if ((1 << x) & image) {
                lcd_set_pixel (aX + 7 - x, y + aY, TRUE);
            }
        }
    }
}

void lcd_paint_string (muint8 aX, muint8 aY, const mchar *aString) {
    mchar ch;

    m_return_if_fail (aString);
    m_return_if_fail (aX < 61);
    m_return_if_fail (aY < 16);

    while (0 != (ch = *aString)) {
        lcd_paint_char (aX, aY, ch);

        aX += 7;
        if (aX > 60) {
            break;
        }

        /* handle the next string */
        ++aString;
    }
}

void lcd_paint_string_p (muint8 aX, muint8 aY, const mchar *aString) {
    mchar ch;

    m_return_if_fail (aString);
    m_return_if_fail (aX < 61);
    m_return_if_fail (aY < 16);

    while (0 != (ch = pgm_read_byte  (aString))) {
        lcd_paint_char (aX, aY, ch);

        aX += 7;
        if (aX > 60) {
            break;
        }

        /* handle the next string */
        ++aString;
    }
}

void lcd_set_pixel (muint8 aX, muint8 aY, muint8 aValue) {
    muint8 bit_mask;

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
