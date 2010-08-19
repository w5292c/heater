#include "fonts.h"
#include "lcd-driver.h"
#include "macros.h"

#include <stdio.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    muint8 sh = 7;
    lcd_paint_char ( 0 + sh, 1, 'H');
    lcd_paint_char ( 7 + sh, 1, 'e');
    lcd_paint_char (14 + sh, 1, 'l');
    lcd_paint_char (21 + sh, 1, 'l');
    lcd_paint_char (28 + sh, 1, 'o');
    lcd_paint_char (35 + sh, 1, '!');
    lcd_set_pixel ( 0,  0, TRUE);
    lcd_set_pixel ( 0, 15, TRUE);
    lcd_set_pixel (60,  0, TRUE);
    lcd_set_pixel (60, 15, TRUE);
    lcd_flash ();

    lcd_deinit ();
    return 0;
}
