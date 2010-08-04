#include "fonts.h"

#include <stdio.h>

int main (char **argv, int argn) {
    muword i, j;
    for (i = 0; i < 14; i++) {
        mubyte bt = get_font14_byte (57, i);
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

    return 0;
}