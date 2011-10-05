#include "scheduler.h"

#include "macros.h"
#include "config.h"

#ifndef M_MAX_TICKS
#error "The macro M_MAX_TICKS is not defined, the scheduler is disabled"
#endif

typedef struct {
    scheduler_tick mTick;
} TTickInfo;

static mbool TheExitRequest = FALSE;
static TTickInfo TheItems[M_MAX_TICKS];

void scheduler_init (void) {
    muint i;

    for (i = 0; i < M_MAX_TICKS; i++) {
        TheItems[i].mTick = NULL;
    }
}

void scheduler_start (void) {
    while (!TheExitRequest) {
        muint i;

        for (i = 0; i < M_MAX_TICKS; i++) {
            scheduler_tick tick;

            tick = TheItems[i].mTick;
            if (tick) {
                (*tick) ();
            }
        }
    }
}

#ifndef M_NO_DEINIT
void scheduler_stop (void) {
    TheExitRequest = TRUE;
}
#endif /* !M_NO_DEINIT */

void scheduler_add (scheduler_tick aTick) {
    muint i;
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
    muint i;
    TTickInfo *info;

    for (i = 0; i < M_MAX_TICKS; i++) {
        info = &TheItems[i];

        if (info->mTick == aTick) {
            info->mTick = NULL;
            break;
        }
    }

    m_return_if_fail (i != M_MAX_TICKS);
}

void scheduler_enter_sleep_mode (void) {
}

void scheduler_leave_sleep_mode (void) {
}
