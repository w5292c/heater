#include "fonts.h"
#include "macros.h"

#include <avr/io.h>

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

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

    return 0;
}
