#include "fonts.h"
#include "lcd-driver.h"
#include "macros.h"

#include <stdio.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    lcd_print_char (0, 0, 'H');
    lcd_print_char (8, 0, 'e');
    lcd_print_char (16, 0, 'l');
    lcd_print_char (24, 0, 'l');
    lcd_print_char (32, 0, 'o');
    lcd_print_char (40, 0, '!');
    lcd_set_pixel (0, 0, TRUE);
    lcd_set_pixel (0, 15, TRUE);
    lcd_set_pixel (60, 0, TRUE);
    lcd_set_pixel (60, 15, TRUE);
    lcd_flash ();

    lcd_debug_show_buffer ();

    lcd_deinit ();
    return 0;
}
