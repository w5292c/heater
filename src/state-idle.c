#include "state-idle.h"

#include "engine.h"
#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

static void state_idle_enter (void);
static void state_idle_rtc_timer (void);
static void state_idle_key_event (muint8 aCode);


void engine_state_idle_init (void) {
    TEngineStateInterface idle_api;

    idle_api.mEnter = state_idle_enter;
    idle_api.mLeave = NULL;
    idle_api.mTick = NULL;
    idle_api.mTimer = NULL;
    idle_api.mRtcTimer = state_idle_rtc_timer;
    idle_api.mKeyEvent = state_idle_key_event;

    engine_register_state (EEngineStateIdle, &idle_api);
}

void engine_state_idle_deinit (void) {
}

static void state_idle_enter (void) {
    state_idle_rtc_timer ();
}

static void state_idle_rtc_timer (void) {
    const TRtcTimeInfo *time = engine_get_current_time ();
    m_return_if_fail (time);

    /* show time */
    lcd_clear ();
    lcd_print_char (0, 1, '0' + ((time->mHour&0xF0U)>>4));
    lcd_paint_char (8, 1, '0' + ((time->mHour&0x0FU)));
    lcd_print_char (15, 1, ':');
    lcd_paint_char (22, 1, '0' + ((time->mMinute&0xF0U)>>4));
    lcd_print_char (30, 1, '0' + ((time->mMinute&0x0FU)));
    lcd_paint_char (37, 1, ':');
    lcd_paint_char (44, 1, '0' + ((time->mSeconds&0xF0U)>>4));
    lcd_paint_char (52, 1, '0' + ((time->mSeconds&0x0FU)));
    lcd_flash ();
}

static void state_idle_key_event (muint8 aCode) {
    if (EHwKeyCodeKey1 == aCode) {
        engine_request_state (EEngineStateDate);
    }
}
