#include "config.h"

#include <avr/io.h>

#ifdef CONFIG_PROGRAM_FUSES

FUSES = {
    .low = LFUSE_DEFAULT,
    .high = HFUSE_DEFAULT,
};

#endif /* CONFIG_PROGRAM_FUSES */
