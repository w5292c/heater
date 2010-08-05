#include "fonts.h"
#include "lcd-driver.h"
#include "macros.h"

#include <stdio.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    uint8_t sh = 7;
    lcd_print_char (0 + sh, 1, 'H');
    lcd_paint_char (8 + sh, 1, 'e');
    lcd_print_char (16 + sh, 1, 'l');
    lcd_paint_char (24 + sh, 1, 'l');
    lcd_print_char (32 + sh, 1, 'o');
    lcd_paint_char (40 + sh, 1, '!');
    lcd_set_pixel (0, 0, TRUE);
    lcd_set_pixel (0, 15, TRUE);
    lcd_set_pixel (60, 0, TRUE);
    lcd_set_pixel (60, 15, TRUE);
    lcd_flash ();

    lcd_debug_show_buffer ();

    lcd_deinit ();
    return 0;
}
