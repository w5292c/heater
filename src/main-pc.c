#include "fonts.h"
#include "lcd-driver.h"
#include "macros.h"

#include <stdio.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    lcd_paint_string_p (7, 1, PSTR ("Hello!"));
    lcd_set_pixel ( 0,  0, TRUE);
    lcd_set_pixel ( 0, 15, TRUE);
    lcd_set_pixel (60,  0, TRUE);
    lcd_set_pixel (60, 15, TRUE);
    lcd_flash ();

    lcd_deinit ();
    return 0;
}
