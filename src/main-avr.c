#include "engine.h"
#include "scheduler.h"

#include <avr/interrupt.h>

int main () {
    /* initialize the engine */
    engine_init ();

    /* the main loop */
    sei ();
    scheduler_start ();

    /* deinitialize the engine */
    engine_deinit ();

    return 0;
}
