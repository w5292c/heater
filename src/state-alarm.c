#include "state-alarm.h"

#include "state-idle.h"

#include "engine.h"
#include "macros.h"
#include "hw-rtc.h"
#include "lcd-driver.h"

static muint16 TheInactivity = 0;

static void state_alarm_enter (void);
static void state_alarm_leave (void);
static void state_alarm_tick (void);
static void state_alarm_timer (void);
static void state_alarm_rtc_timer (void);
static void state_alarm_key_event (muint8 aCode);


void engine_state_alarm_init (void) {
    TEngineStateInterface idle_api;

    idle_api.mEnter = state_alarm_enter;
    idle_api.mLeave = state_alarm_leave;
    idle_api.mTick = state_alarm_tick;
    idle_api.mTimer = state_alarm_timer;
    idle_api.mRtcTimer = state_alarm_rtc_timer;
    idle_api.mKeyEvent = state_alarm_key_event;

    engine_register_state (EEngineStateAlarm, &idle_api);
}

void engine_state_alarm_deinit (void) {
}

static void state_alarm_enter (void) {
    lcd_clear ();
    lcd_print_char (0, 1, 'A');
    lcd_paint_char (8, 1, 'l');
    lcd_print_char (16, 1, 'a');
    lcd_paint_char (24, 1, 'r');
    lcd_print_char (32, 1, 'm');
    lcd_flash ();
}

static void state_alarm_leave (void) {
}

static void state_alarm_tick (void) {
}

static void state_alarm_timer (void) {
    ++TheInactivity;
    if (4000 == TheInactivity) {
        TheInactivity = 0;

        engine_request_state (EEngineStateIdle);
    }
}

static void state_alarm_rtc_timer (void) {
}

static void state_alarm_key_event (muint8 aCode) {
    M_UNUSED_PARAM (aCode);

    TheInactivity = 0;
}
