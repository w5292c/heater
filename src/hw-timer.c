#include "hw-timer.h"

#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <avr/io.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

typedef struct {
    hw_timer_callback mCallback;
} THwTimerTickInfo;

static muint8 volatile TheTimerTickPending;
static THwTimerTickInfo TheTimerTickInfos[M_MAX_TIMER_TICKS];

static muint8 hw_timer_tick (void);
/**
 * Initialize the Timer0-related HW registers
 */
static inline void hw_timer_init_timer0 (void) {
    /* Reset the Timer0 counter */
    TCNT0 = 0x00U;
    /* Timer0 compare register */
    OCR0 = 0xFAU;
    /* TimerClock = SystemClock / 64 */
    TCCR0 = (1<<CS01) | (1<<CS00);
    /* Clear OCF0 / clear pending interrupts */
    TIFR  = (1<<OCF0);
    /* Enable Timer0 Compare Interrupt */
    TIMSK = (1<<OCIE0);
}

void hw_timer_init (void) {
    muint8 i;

    TheTimerTickPending = FALSE;
    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        TheTimerTickInfos[i].mCallback = NULL;
    }
    scheduler_add (&hw_timer_tick);

    hw_timer_init_timer0 ();
}

void hw_timer_deinit (void) {
    scheduler_remove (&hw_timer_tick);
    TheTimerTickPending = FALSE;
}

void hw_timer_add_callback (hw_timer_callback aCallback) {
    muint8 i;
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

void hw_timer_remove_callback (hw_timer_callback aCallback) {
    muint8 i;
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

static muint8 hw_timer_tick (void) {
    if (TheTimerTickPending) {
        muint8 i;
        hw_timer_callback callback;

        /* dispatch another timer tick */
        for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
            callback = TheTimerTickInfos[i].mCallback;
            if (callback) {
                (*callback) ();
            }
        }

        /* reset the pending flag */
        TheTimerTickPending = FALSE;
    }

    return FALSE;
}

/* This is a 1 KHz timer0 handler */
ISR (TIMER0_COMP_vect) {
    /* set the flag */
    TheTimerTickPending = TRUE;

    /* reset the counter */
    TCNT0 = 0x00;
}
