#include "hw-keys.h"

#include "macros.h"
#include "hw-timer.h"

/**
 * The handler for a 1ms timeout event
 */
static void hw_keys_timer_callback (void);

void hw_keys_init (void) {
    hw_timer_add_callback (&hw_keys_timer_callback);
}

void hw_keys_deinit (void) {
    hw_timer_remove_callback (&hw_keys_timer_callback);
}

void hw_keys_add_key_event_handler (hw_keys_on_key_event aCallback) {
    M_UNUSED_PARAM (aCallback);
}

void hw_keys_remove_key_event_handler (hw_keys_on_key_event aCallback) {
    M_UNUSED_PARAM (aCallback);
}

static void hw_keys_timer_callback (void) {
}
