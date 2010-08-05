#include "fonts.h"

#ifdef M_PC
#include <stdio.h>
#else
#include <avr/io.h>
#endif

int main (char **argv, int argn) {
    muword i, j;
    for (i = 0; i < 14; i++) {
        mubyte bt = get_font14_byte (126, i);
#ifdef M_PC
        printf ("[");
#endif
        for (j = 0x80U; j != 0; j = j >>1) {
            if (bt & j) {
#ifdef M_PC
                printf ("XX");
#endif
            }
            else {
#ifdef M_PC
                printf ("  ");
#endif
            }
        }
#ifdef M_PC
        printf ("]\n");
#endif
    }

    return 0;
}