#include "fonts.h"

#include <avr/io.h>

int main (char **argv, int argn) {
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