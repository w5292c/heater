#include "engine.h"

#include "macros.h"
#include "hw-rtc.h"
#include "hw-keys.h"
#include "hw-timer.h"
#include "scheduler.h"
#include "lcd-driver.h"
#include "state-idle.h"
#include "state-alarm.h"

#ifdef M_AVR
#include <string.h>
#endif /* M_AVR */

static muint8 TheCurrentState = EEngineStateNull;
static TEngineStateInterface TheStates[EEngineStateLast] = {{NULL,NULL,NULL,NULL,NULL,NULL},};
static TRtcTimeInfo TheCurrentTime = {0,0,0,0,0,0,0};

/**
 * The task scheduler tick for the engine
 * @param[in] aData The engine pointer
 * @return More work is pending
 */
static mbool engine_tick (void);
/**
 * 1 KHz timer tick
 */
static void engine_timer_tick (void);
static void engine_rtc_timer (void);
static void engine_on_key_event (muint8 aCode);
static void engine_show_hello (void);
static void engine_rtc_time_ready (const TRtcTimeInfo *aTimeInfo);

void engine_init (void) {
    scheduler_add (&engine_tick);
    hw_timer_init ();
    hw_timer_add_callback (&engine_timer_tick);
    hw_rtc_init ();
    hw_rtc_set_rtc_timer (&engine_rtc_timer);
    hw_keys_init ();
    hw_keys_add_key_event_handler (&engine_on_key_event);
    lcd_init ();
    engine_show_hello ();

    /* the last thing, initialize the states */
    engine_state_idle_init ();
    engine_state_alarm_init ();

    /* Request initial state */
    engine_request_state (EEngineStateIdle);
}

void engine_deinit (void) {
    /* first deinitialize the states */
    engine_state_alarm_deinit ();
    engine_state_idle_deinit ();

    hw_keys_deinit ();
    hw_keys_remove_key_event_handler (&engine_on_key_event);
    hw_rtc_set_rtc_timer (NULL);
    hw_rtc_deinit ();
    hw_timer_remove_callback (&engine_timer_tick);
    hw_timer_deinit ();
    scheduler_remove (&engine_tick);

    /* deinitialize the LCD interface */
    lcd_deinit ();
}

void engine_register_state (muint8 aEngineState, TEngineStateInterface *aInterface) {
    m_return_if_fail (aEngineState < EEngineStateLast);

    memcpy (&TheStates[aEngineState], aInterface, sizeof (TEngineStateInterface));
}

void engine_request_state (muint8 aNewState) {
    m_return_if_fail (aNewState < EEngineStateLast);

    if (TheCurrentState != aNewState) {
        engine_state_enter enter;

        /* leave the previous state */
        if (EEngineStateNull != TheCurrentState) {
            engine_state_leave leave;

            leave = TheStates[TheCurrentState].mLeave;
            if (leave) {
                (*leave) ();
            }
        }

        /* enter the new state */
        enter = TheStates[aNewState].mEnter;
        if (enter) {
            (*enter) ();
        }

        TheCurrentState = aNewState;
    }
}

const TRtcTimeInfo *engine_get_current_time (void) {
    return &TheCurrentTime;
}

static mbool engine_tick (void) {
    engine_state_tick tick;

    tick = TheStates[TheCurrentState].mTick;
    if (tick) {
        (*tick) ();
    }

    return TRUE;
}

static void engine_rtc_timer (void) {
    hw_rtc_get_time (&engine_rtc_time_ready);
}

static void engine_rtc_time_ready (const TRtcTimeInfo *aTimeInfo) {
    engine_state_rtc_timer rtc_timer;

    memcpy (&TheCurrentTime, aTimeInfo, sizeof (TRtcTimeInfo));

    rtc_timer = TheStates[TheCurrentState].mRtcTimer;
    if (rtc_timer) {
        (*rtc_timer) ();
    }
}

static void engine_timer_tick (void) {
    engine_state_timer timer;

    timer = TheStates[TheCurrentState].mTimer;
    if (timer) {
        (*timer) ();
    }
}

static void engine_on_key_event (muint8 aCode) {
    engine_state_key_event key_event;

    key_event = TheStates[TheCurrentState].mKeyEvent;
    if (key_event) {
        (*key_event) (aCode);
    }
}

static void engine_show_hello (void) {
    const muint8 sh = 7;

    lcd_clear ();
    lcd_print_char (0 + sh, 1, 'H');
    lcd_paint_char (8 + sh, 1, 'e');
    lcd_print_char (16 + sh, 1, 'l');
    lcd_paint_char (24 + sh, 1, 'l');
    lcd_print_char (32 + sh, 1, 'o');
    lcd_paint_char (40 + sh, 1, '!');
    lcd_set_pixel (0, 0, TRUE);
    lcd_set_pixel (0, 15, TRUE);
    lcd_set_pixel (60, 0, TRUE);
    lcd_set_pixel (60, 15, TRUE);
    lcd_flash ();
}
