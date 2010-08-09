#include "hw-timer.h"

#include "macros.h"
#include "scheduler.h"

typedef struct {
    hw_timer_callback mCallback;
} THwTimerTickInfo;

static uint8_t volatile TheTimerTickPending;
static THwTimerTickInfo TheTimerTickInfos[M_MAX_TIMER_TICKS];

static uint8_t hw_timer_tick (void);

void hw_timer_init (void) {
    uint8_t i;

    TheTimerTickPending = FALSE;
    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        TheTimerTickInfos[i].mCallback = NULL;
    }
    scheduler_add (&hw_timer_tick);
}

void hw_timer_deinit (void) {
    scheduler_remove (&hw_timer_tick);
    TheTimerTickPending = FALSE;
}

void hw_timer_add_tick (hw_timer_callback aCallback) {
    uint8_t i;
    THwTimerTickInfo *info;

    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        info = &TheTimerTickInfos[i];

        if (!info->mCallback) {
            info->mCallback = aCallback;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TIMER_TICKS);
}

void hw_timer_remove_tick (hw_timer_callback aCallback) {
    uint8_t i;
    THwTimerTickInfo *info;

    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        info = &TheTimerTickInfos[i];

        if (info->mCallback == aCallback) {
            info->mCallback = NULL;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TIMER_TICKS);
}

static uint8_t hw_timer_tick (void) {
    if (TheTimerTickPending) {
        uint8_t i;
        THwTimerTickInfo *info;

        /* dispatch another timer tick */
        for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
            info = &TheTimerTickInfos[i];

            if (!info->mCallback) {
                (*info->mCallback) ();
            }
        }

        /* reset the pending flag */
        TheTimerTickPending = FALSE;
    }

    return FALSE;
}
