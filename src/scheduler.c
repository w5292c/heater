#include "scheduler.h"

#include "macros.h"
#include "config.h"

#ifndef M_MAX_TICKS
#error "The macro M_MAX_TICKS is not defined, the scheduler is disabled"
#endif

typedef struct {
    scheduler_tick mTick;
} TTickInfo;

static uint8_t TheExitRequest = FALSE;
static TTickInfo TheItems[M_MAX_TICKS];

void scheduler_init (void) {
    uint8_t i;

    for (i = 0; i < M_MAX_TICKS; i++) {
        TheItems[i].mTick = 0;
    }
}

void scheduler_start (void) {
    while (!TheExitRequest) {
        uint8_t i;

        for (i = 0; i < M_MAX_TICKS; i++) {
            scheduler_tick tick;

            tick = TheItems[i].mTick;
            if (tick) {
                (*tick) ();
            }
        }
    }
}

void scheduler_stop (void) {
    TheExitRequest = TRUE;
}

void scheduler_add (scheduler_tick aTick) {
    uint8_t i;
    TTickInfo *info;

    for (i = 0; i < M_MAX_TICKS; i++) {
        info = &TheItems[i];

        if (!info->mTick) {
            info->mTick = aTick;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TICKS);
}

void scheduler_remove (scheduler_tick aTick) {
    uint8_t i;
    TTickInfo *info;

    for (i = 0; i < M_MAX_TICKS; i++) {
        info = &TheItems[i];

        if (info->mTick == aTick) {
            info->mTick = 0;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TICKS);
}
