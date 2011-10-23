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

#include "state-set-time.h"

#include "editor.h"
#include "engine.h"
#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#endif

/**
 * The internal states for the set-time state
 */
typedef enum {
    ESetTimeStateNull = 0,
    ESetTimeStateIdle,
    ESetTimeStateIntro,
    ESetTimeStateEditor,
    ESetTimeStateLeaving
} TSetTimeState;

static muint8 TheSetTimeState = ESetTimeStateNull;
static muint16 TheSetTimeInactivityTimer = 0;

static void state_set_time_enter (void);
static void state_set_time_leave (void);
static void state_set_time_timer (void);
static void state_set_time_key_event (muint8 aCode);
static void state_set_time_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo);

void engine_state_set_time_init (void) {
    TEngineStateInterface set_time_api;

    /* The state should only be intialized once */
    m_return_if_fail (ESetTimeStateNull == TheSetTimeState);

    set_time_api.mEnter = state_set_time_enter;
    set_time_api.mLeave = state_set_time_leave;
    set_time_api.mTick = NULL;
    set_time_api.mTimer = state_set_time_timer;
    set_time_api.mRtcTimer = NULL;
    set_time_api.mKeyEvent = state_set_time_key_event;
    TheSetTimeState = ESetTimeStateIdle;

    engine_register_state (EEngineStateTimeSet, &set_time_api);
}

#ifndef M_NO_DEINIT
void engine_state_set_time_deinit (void) {
    TheSetTimeState = ESetTimeStateNull;
}
#endif /* !M_NO_DEINIT */

static void state_set_time_enter (void) {
    TheSetTimeState = ESetTimeStateIntro;

    lcd_clear ();
    lcd_paint_string_7x14_p (0, 0, PSTR ("Set time"));
    lcd_flash ();
}

static void state_set_time_leave (void) {
    TheSetTimeInactivityTimer = 0;
    TheSetTimeState = ESetTimeStateIdle;
}

static void state_set_time_activate_editor (void) {
    const TRtcTimeInfo *const time = engine_get_current_time ();
    m_return_if_fail (time);

    editor_activate (0, &state_set_time_editor_done, time);
    TheSetTimeState = ESetTimeStateEditor;
}

static void state_set_time_timer (void) {
    switch (TheSetTimeState)
    {
    case ESetTimeStateIntro:
        if (1000 == TheSetTimeInactivityTimer) {
            state_set_time_activate_editor ();
            TheSetTimeInactivityTimer = 0;
        }
        break;
    case ESetTimeStateEditor:
        if (10000 == TheSetTimeInactivityTimer) {
            TheSetTimeState = ESetTimeStateLeaving;

            editor_deactivate ();
            engine_request_state (EEngineStateIdle);
        }
        break;
    case ESetTimeStateNull:
    case ESetTimeStateIdle:
    case ESetTimeStateLeaving:
        break;
    }

    /* Update the inactivity counter */
    ++TheSetTimeInactivityTimer;
}

static void state_set_time_key_event (muint8 aCode) {
    if (EHwKeyCodeKey1 == aCode) {
        if (ESetTimeStateIntro == TheSetTimeState) {
            engine_request_state (EEngineStateIdle);
        }
    }

    TheSetTimeInactivityTimer = 0;
}

static void state_set_time_time_updated (mbool aSuccess) {
    m_assert (aSuccess, PSTR("No update"));
    engine_request_state (EEngineStateIdle);
}

static void state_set_time_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo) {
    m_return_if_fail (aInfo);
    editor_deactivate ();
    if (aConfirmed) {
        const TRtcTimeInfo *currentTime = engine_get_current_time ();
        m_return_if_fail (currentTime);
        TRtcTimeInfo newTime = *currentTime;
        newTime.mSeconds = 0;
        newTime.mHour = aInfo->mHour;
        newTime.mMinute = aInfo->mMinute;
        hw_rtc_set_time (&newTime, &state_set_time_time_updated);
    }
}
