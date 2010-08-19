#include "state-alarm.h"

#include "editor.h"
#include "engine.h"
#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#endif

/**
 * The internal state for the alarm state
 */
typedef enum {
    EAlarmStateNull = 0,
    EAlarmStateIdle,
    EAlarmStateIntro,
    EAlarmStateTimeEditor,
    EAlarmStateLeaving
} TAlarmState;

static muint8 TheAlarmState = EAlarmStateNull;
static muint16 TheInactivityTimer = 0;

static void state_alarm_enter (void);
static void state_alarm_leave (void);
static void state_alarm_timer (void);
static void state_alarm_key_event (muint8 aCode);
static void state_alarm_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo);

void engine_state_alarm_init (void) {
    TEngineStateInterface alarm_api;

    /* The state should only be intialized once */
    m_return_if_fail (EAlarmStateNull == TheAlarmState);

    alarm_api.mEnter = state_alarm_enter;
    alarm_api.mLeave = state_alarm_leave;
    alarm_api.mTick = NULL;
    alarm_api.mTimer = state_alarm_timer;
    alarm_api.mRtcTimer = NULL;
    alarm_api.mKeyEvent = state_alarm_key_event;
    TheAlarmState = EAlarmStateIdle;

    engine_register_state (EEngineStateAlarm, &alarm_api);
}

void engine_state_alarm_deinit (void) {
    TheAlarmState = EAlarmStateNull;
}

static void state_alarm_enter (void) {
    TheAlarmState = EAlarmStateIntro;

    lcd_clear ();
    lcd_paint_string_p (0, 0, PSTR ("Alarm"));
    lcd_flash ();
}

static void state_alarm_leave (void) {
    TheInactivityTimer = 0;
    TheAlarmState = EAlarmStateIdle;
}

static void state_alarm_activate_editor (void) {
    TRtcTimeInfo info;
    info.mMinute = 0x52;
    info.mHour = 0x15;

    editor_activate (0, &state_alarm_editor_done, &info);
    TheAlarmState = EAlarmStateTimeEditor;
}

static void state_alarm_timer (void) {
    switch (TheAlarmState)
    {
    case EAlarmStateIntro:
        if (1000 == TheInactivityTimer) {
            state_alarm_activate_editor ();
            TheInactivityTimer = 0;
        }
        break;
    case EAlarmStateTimeEditor:
        if (4000 == TheInactivityTimer) {
            TheAlarmState = EAlarmStateLeaving;

            editor_deactivate ();
            engine_request_state (EEngineStateIdle);
        }
        break;
    case EAlarmStateNull:
    case EAlarmStateIdle:
    case EAlarmStateLeaving:
        break;
    }

    /* Update the inactivity counter */
    ++TheInactivityTimer;
}

static void state_alarm_key_event (muint8 aCode) {
    if (EHwKeyCodeKey1 == aCode) {
        if (EAlarmStateIntro == TheAlarmState) {
            TheAlarmState = EAlarmStateLeaving;

            engine_request_state (EEngineStateIdle);
        }
        else {
            TheInactivityTimer = 0;
        }
    }
    else {
        state_alarm_activate_editor ();
        TheInactivityTimer = 0;
    }
}

static void state_alarm_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo) {
    M_UNUSED_PARAM (aInfo);
    M_UNUSED_PARAM (aConfirmed);
}
