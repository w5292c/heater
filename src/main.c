#include "fonts.h"

#include <stdio.h>

int main (char **argv, int argn) {
    muword i;
    for (i = 0; i < 14; i++) {
        mubyte bt = get_font14_byte (100, i);
        printf ("Value: %d\n", bt);
    }

    return 0;
}