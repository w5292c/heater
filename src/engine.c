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

#include "engine.h"

#include "alarm.h"
#include "macros.h"
#include "editor.h"
#include "hw-rtc.h"
#include "hw-keys.h"
#include "hw-timer.h"
#include "hw-sound.h"
#include "scheduler.h"
#include "lcd-driver.h"
#include "state-idle.h"
#include "state-date.h"
#include "state-alarm.h"
#include "at-cmd-engine.h"
#include "state-set-time.h"

#ifdef M_AVR
#include <string.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
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
    editor_init ();
    hw_sound_init ();
    at_engine_init ();

    /* the last thing, initialize the states */
    engine_state_idle_init ();
    engine_state_date_init ();
    engine_state_alarm_init ();
    engine_state_set_time_init ();

    /* Request initial state */
    engine_request_state (EEngineStateIdle);

#if 0 /* do not play a melody at start-up, can be enabled for testing */
    alarm_play_melody (EAlarmMelodyCTree, NULL);
#endif
}

#ifndef M_NO_DEINIT
void engine_deinit (void) {
    /* first deinitialize the states */
    engine_state_set_time_deinit ();
    engine_state_alarm_deinit ();
    engine_state_date_deinit ();
    engine_state_idle_deinit ();

    at_engine_deinit ();
    hw_sound_deinit ();
    editor_deinit ();
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
#endif /* !M_NO_DEINIT */

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
    const TRtcTimeInfo KNullTime = {0,0,0,0,0,0,0};

    if (0 == memcmp (&KNullTime, &TheCurrentTime, sizeof (TRtcTimeInfo))) {
        return NULL;
    }
    else {
        return &TheCurrentTime;
    }
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
    lcd_clear ();

    if (MCUCSR & (1<<WDRF)) {
        /* watchdog reset, display 'Error' intro */
        lcd_paint_string_7x14_p (0, 1, PSTR ("Error!"));
        lcd_paint_string_5x8_p (42, 0, PSTR ("\200A\201"));
        lcd_paint_string_5x8_p (42, 8, PSTR ("\200B\201"));
    }
    else {
        /* one of ordinary resets */
        lcd_paint_string_7x14_p (0, 1, PSTR ("Hello!"));
        lcd_paint_string_5x8_p (42, 0, PSTR ("\200C\201"));
        lcd_paint_string_5x8_p (42, 8, PSTR ("\200D\201"));
    }
    lcd_flash ();
}

void engine_panic_p (const char *aPanicInfo) {
    /* disable the interrupts and the watchdog timer */
    cli ();
    wdt_disable ();

    /* show the panic info */
    lcd_clear ();
    if (aPanicInfo) {
        lcd_paint_string_7x14_p (0, 0, aPanicInfo);
    }
    else {
        lcd_paint_string_7x14_p (0, 0, PSTR("<null>"));
    }
    lcd_flash ();
    /* put the CPU into the power-down mode, stop here */
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    sleep_enable ();
    sleep_cpu ();
}
