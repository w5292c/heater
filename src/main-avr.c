#include "fonts.h"
#include "macros.h"
#include "lcd-driver.h"

#include <avr/io.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    lcd_print_char (0, 0, 'F');
    lcd_flash ();

    lcd_deinit ();
    return 0;
}

#if 0
    muword i, j;
    for (i = 0; i < 14; i++) {
        mubyte bt = get_font14_byte (126, i);
        for (j = 0x80U; j != 0; j = j >>1) {
            if (bt & j) {
            }
            else {
            }
        }
    }
#endif
