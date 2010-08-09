#include "hw-timer.h"

#include "macros.h"

typedef struct {
    hw_timer_tick mTick;
} THwTimerTickInfo;

static THwTimerTickInfo TheTimerTickInfos[M_MAX_TIMER_TICKS];

void hw_timer_init (void) {
}

void hw_timer_deinit (void) {
}

void hw_timer_add_tick (hw_timer_tick aTimerTick) {
    uint8_t i;
    THwTimerTickInfo *info;

    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        info = &TheTimerTickInfos[i];

        if (!info->mTick) {
            info->mTick = aTimerTick;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TIMER_TICKS);
}

void hw_timer_remove_tick (hw_timer_tick aTimerTick) {
    uint8_t i;
    THwTimerTickInfo *info;

    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        info = &TheTimerTickInfos[i];

        if (info->mTick == aTimerTick) {
            info->mTick = NULL;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TIMER_TICKS);
}
