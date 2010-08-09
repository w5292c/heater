#include "hw-rtc.h"

#include "macros.h"
#include "scheduler.h"

static hw_rtc_time_ready TheCallback = NULL;

static uint8_t hw_rtc_tick (void);

void hw_rtc_init (void) {
    TheCallback = NULL;

    scheduler_add (&hw_rtc_tick);
}

void hw_rtc_deinit (void) {
    scheduler_remove (&hw_rtc_tick);

    TheCallback = NULL;
}

void hw_rtc_get_time (hw_rtc_time_ready aCallback) {
    m_return_if_fail (TheCallback);

    TheCallback = aCallback;
}

static uint8_t hw_rtc_tick (void) {
    if (TheCallback) {
        (*TheCallback) ();
        TheCallback = NULL;
    }

    return FALSE;
}
