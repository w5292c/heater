#include "macros.h"
#include "engine.h"
#include "scheduler.h"

int main (int argn, char **argv) {
    M_UNUSED_PARAM (argn);
    M_UNUSED_PARAM (argv);

    /* initialize the engine */
    engine_init ();

    /* the main loop */
    scheduler_start ();

    /* deinitialize the engine */
    engine_deinit ();

    return 0;
}
