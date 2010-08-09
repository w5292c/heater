#include "hw-i2c.h"

#include "macros.h"
#include "scheduler.h"

static hw_i2c_time_ready TheCallback = NULL;

static uint8_t hw_i2c_tick (void);

void hw_i2c_init (void) {
    TheCallback = NULL;

    scheduler_add (&hw_i2c_tick);
}

void hw_i2c_deinit (void) {
    scheduler_remove (&hw_i2c_tick);

    TheCallback = NULL;
}

void hw_i2c_get_time (hw_i2c_time_ready aCallback) {
    m_return_if_fail (TheCallback);

    TheCallback = aCallback;
}

static uint8_t hw_i2c_tick (void) {
    if (TheCallback) {
        (*TheCallback) ();
        TheCallback = NULL;
    }

    return FALSE;
}
