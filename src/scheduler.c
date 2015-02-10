/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Alexander Chumakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
