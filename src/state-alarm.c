#include "state-alarm.h"

#include "editor.h"
#include "engine.h"
#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

/**
 * The internal state for the alarm state
 */
typedef enum {
    EAlarmStateNull = 0,
    EAlarmStateIdle,
    EAlarmStateIntro,
    EAlarmStateTimeEditor
} TAlarmState;

static muint8 TheAlarmState = EAlarmStateNull;
static muint16 TheInactivityTimer = 0;

static void state_alarm_enter (void);
static void state_alarm_leave (void);
static void state_alarm_tick (void);
static void state_alarm_timer (void);
static void state_alarm_rtc_timer (void);
static void state_alarm_key_event (muint8 aCode);
static void state_alarm_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo);

void engine_state_alarm_init (void) {
    TEngineStateInterface alarm_api;

    alarm_api.mEnter = state_alarm_enter;
    alarm_api.mLeave = state_alarm_leave;
    alarm_api.mTick = state_alarm_tick;
    alarm_api.mTimer = state_alarm_timer;
    alarm_api.mRtcTimer = state_alarm_rtc_timer;
    alarm_api.mKeyEvent = state_alarm_key_event;
    TheAlarmState = EAlarmStateIdle;

    engine_register_state (EEngineStateAlarm, &alarm_api);
}

void engine_state_alarm_deinit (void) {
}

static void state_alarm_enter (void) {
    TheAlarmState = EAlarmStateIntro;

    lcd_clear ();
    lcd_print_char (0, 1, 'A');
    lcd_paint_char (8, 1, 'l');
    lcd_print_char (16, 1, 'a');
    lcd_paint_char (24, 1, 'r');
    lcd_print_char (32, 1, 'm');
    lcd_flash ();
}

static void state_alarm_leave (void) {
    TheInactivityTimer = 0;
    TheAlarmState = EAlarmStateIdle;
}

static void state_alarm_tick (void) {
}

static void state_alarm_timer (void) {
    switch (TheAlarmState)
    {
    case EAlarmStateIntro:
        if (1000 == TheInactivityTimer) {
            TRtcTimeInfo info;
            info.mMinute = 0x52;
            info.mHour = 0x15;

            TheInactivityTimer = 0;
            editor_activate (0, &state_alarm_editor_done, &info);
            TheAlarmState = EAlarmStateTimeEditor;
        }
        break;
    case EAlarmStateTimeEditor:
        if (3000 == TheInactivityTimer) {
            engine_request_state (EEngineStateIdle);
            editor_deactivate ();
        }
        break;
    }

    ++TheInactivityTimer;
    if (4000 == TheInactivityTimer) {
        TheInactivityTimer = 0;

        engine_request_state (EEngineStateIdle);
        editor_deactivate ();
    }
}

static void state_alarm_rtc_timer (void) {
}

static void state_alarm_key_event (muint8 aCode) {
    if (EHwKeyCodeKey1 == aCode) {
        if (EAlarmStateIntro == TheAlarmState) {
            engine_request_state (EEngineStateIdle);
            editor_deactivate ();
        }
        else {
            TheInactivityTimer = 0;
        }
    }
    else {
        TheInactivityTimer = 0;
    }
}

static void state_alarm_editor_done (mbool aConfirmed, TRtcTimeInfo *aInfo) {
}
