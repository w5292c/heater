#include "state-date.h"

#include "engine.h"
#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#endif

static const mint8 TheMonths[] M_FLASH = {
    'J', 'a', 'n',
    'F', 'e', 'b',
    'M', 'a', 'r',
    'A', 'p', 'r',
    'M', 'a', 'y',
    'J', 'u', 'n',
    'J', 'u', 'l',
    'A', 'u', 'g',
    'S', 'e', 'p',
    'O', 'c', 't',
    'N', 'o', 'v',
    'D', 'e', 'c'
};

/**
 * The internal state for the alarm state
 */
typedef enum {
    EDateStateNull = 0,
    EDateStateIdle,
    EDateStateIntro,
    EDateStateDisplayingDate,
    EDateStateLeaving
} TDateState;

static muint8 TheDateState = EDateStateNull;
static muint16 TheDateInactivityTimer = 0;

static void state_date_enter (void);
static void state_date_leave (void);
static void state_date_timer (void);
static void state_date_key_event (muint8 aCode);

void engine_state_date_init (void) {
    TEngineStateInterface alarm_api;

    /* The state should only be intialized once */
    m_return_if_fail (EDateStateNull == TheDateState);

    alarm_api.mEnter = state_date_enter;
    alarm_api.mLeave = state_date_leave;
    alarm_api.mTick = NULL;
    alarm_api.mTimer = state_date_timer;
    alarm_api.mRtcTimer = NULL;
    alarm_api.mKeyEvent = state_date_key_event;
    TheDateState = EDateStateIdle;

    engine_register_state (EEngineStateDate, &alarm_api);
}

void engine_state_date_deinit (void) {
    TheDateState = EDateStateNull;
}

static void state_date_enter (void) {
    TheDateState = EDateStateIntro;

    lcd_clear ();
    lcd_print_char (0, 1, 'D');
    lcd_paint_char (8, 1, 'a');
    lcd_print_char (16, 1, 't');
    lcd_paint_char (24, 1, 'e');
    lcd_flash ();
}

static void state_date_leave (void) {
    TheDateInactivityTimer = 0;
    TheDateState = EDateStateIdle;
}

static void state_date_update (void) {
    const mint8 *month;
    const TRtcTimeInfo *time;

    TheDateState = EDateStateDisplayingDate;

    time = engine_get_current_time ();
    m_return_if_fail (time);

    /* show data */
    lcd_clear ();
    m_return_if_fail (time->mMonth > 0 && time->mMonth < 13);
    month = &TheMonths[(time->mMonth - 1)*3];
    lcd_print_char ( 0, 1, pgm_read_byte (month + 0));
    lcd_paint_char ( 8, 1, pgm_read_byte (month + 1));
    lcd_paint_char (16, 1, pgm_read_byte (month + 2));
    lcd_paint_char (24, 1, '-');
    lcd_paint_char (32, 1, '0' + ((time->mDay&0xF0U)>>4));
    lcd_paint_char (40, 1, '0' + ((time->mDay&0x0FU)));
    lcd_flash ();
}

static void state_date_timer (void) {
    switch (TheDateState)
    {
    case EDateStateIntro:
        if (1000 == TheDateInactivityTimer) {
            state_date_update ();
            TheDateInactivityTimer = 0;
        }
        break;
    case EDateStateDisplayingDate:
        if (4000 == TheDateInactivityTimer) {
            TheDateState = EDateStateLeaving;

            engine_request_state (EEngineStateIdle);
        }
        break;
    case EDateStateNull:
    case EDateStateIdle:
    case EDateStateLeaving:
        break;
    }

    /* Update the inactivity counter */
    ++TheDateInactivityTimer;
}

static void state_date_key_event (muint8 aCode) {
    if (EHwKeyCodeKey1 == aCode) {
        if (EDateStateIntro == TheDateState) {
            engine_request_state (EEngineStateAlarm);
        }
        else if (EDateStateDisplayingDate == TheDateState) {
            engine_request_state (EEngineStateIdle);
        }
    }
}
