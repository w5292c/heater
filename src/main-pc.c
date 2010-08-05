#include "fonts.h"
#include "lcd-driver.h"
#include "macros.h"

#include <stdio.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    lcd_init ();
    lcd_clear ();

    lcd_set_pixel (50, 8, TRUE);
    lcd_print_char (0, 0, 'F');
    lcd_flash ();

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

    lcd_deinit ();
    return 0;
}

#if 0
    do {
        muword i, j;
        int32_t enter;

        printf ("Enter the code to be displayed: ");
        scanf ("%d", &enter);
        printf ("\nThe char: [%d]\n", enter);

        for (i = 0; i < 14; i++) {
            mubyte bt = get_font14_byte (enter, i);
            printf ("[%2.2x] - ", bt);
            printf ("[");
            for (j = 0x80U; j != 0; j = j >>1) {
                if (bt & j) {
                    printf ("XX");
                }
                else {
                    printf ("  ");
                }
            }
            printf ("]\n");
        }
    }
    while (1);
#endif
