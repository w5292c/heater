#include "hw-timer.h"

#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

typedef struct {
    hw_timer_callback mCallback;
} THwTimerTickInfo;

static mbool volatile TheTimerTickPending;
static THwTimerTickInfo TheTimerTickInfos[M_MAX_TIMER_TICKS];

static mbool hw_timer_tick (void);
/**
 * Initialize the Timer2-related HW registers
 */
static inline void hw_timer_init_timer2 (void) {
    /* Reset the Timer2 counter */
    TCNT2 = 0x00U;
    /* Timer2 compare register */
    OCR2 = 249;
    /* Clear OCF2 / clear pending interrupts */
    TIFR  = (1<<OCF2);
    /* Enable Timer2 Compare Interrupt */
    TIMSK = (1<<OCIE2);
    /* Set the control register */
    TCCR2 =
        (1<<WGM21)|(0<<WGM20)| /*< Mode: CTC */
        (0<<COM21)|(0<<COM21)| /*< No port output */
        (1<<CS22)|(0<<CS21)|(0<<CS20); /* Prescaler: 64 */

    /* enable the watchdog timer */
    wdt_enable (WDTO_15MS);
}

void hw_timer_init (void) {
    muint i;

    TheTimerTickPending = FALSE;
    for (i = 0; i < M_MAX_TIMER_TICKS; i++) {
        TheTimerTickInfos[i].mCallback = NULL;
    }
    scheduler_add (&hw_timer_tick);

    hw_timer_init_timer2 ();
}

void hw_timer_deinit (void) {
    scheduler_remove (&hw_timer_tick);
    TheTimerTickPending = FALSE;
}

void hw_timer_add_callback (hw_timer_callback aCallback) {
    muint i;
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
    muint i;
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

static mbool hw_timer_tick (void) {
    if (TheTimerTickPending) {
        muint i;
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
        wdt_reset ();
    }

    return FALSE;
}

/* This is a 1 KHz timer2 handler */
ISR (TIMER2_COMP_vect) {
    /* set the flag */
    TheTimerTickPending = TRUE;
}
