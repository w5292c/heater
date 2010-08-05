#include "fonts.h"

#include <stdio.h>

int main (char **argv, int argn) {

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

    return 0;
}