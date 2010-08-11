#include "engine.h"
#include "macros.h"
#include "scheduler.h"

#include <avr/interrupt.h>

int main (int argc, char **argv) {
    M_UNUSED_PARAM (argc);
    M_UNUSED_PARAM (argv);

    /* initialize the engine */
    engine_init ();

    /* the main loop */
    sei ();
    scheduler_start ();

    /* deinitialize the engine */
    engine_deinit ();

    return 0;
}
