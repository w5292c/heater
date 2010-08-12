#include "engine.h"

#include "macros.h"
#include "hw-rtc.h"
#include "hw-timer.h"
#include "scheduler.h"
#include "lcd-driver.h"

#ifdef M_AVR
#include <string.h>
#endif /* M_AVR */

typedef enum {
    EEngineStateInit,
    EEngineStateState1,
    EEngineStateState2,
    EEngineStateState3,
    EEngineStateState4,
    EEngineStateDone
} TEngineState;

static TEngineState TheEngineState = EEngineStateInit;

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
static void engine_rtc_time_ready (const TRtcTimeInfo *aTimeInfo);

void engine_init (void) {
    scheduler_add (&engine_tick);
    hw_timer_init ();
    hw_timer_add_callback (&engine_timer_tick);
    hw_rtc_init ();
}

void engine_deinit (void) {
    hw_rtc_deinit ();
    hw_timer_remove_callback (&engine_timer_tick);
    hw_timer_deinit ();
    scheduler_remove (&engine_tick);

    /* deinitialize the LCD interface */
    lcd_deinit ();
}

static mbool engine_tick (void) {
    mbool more = TRUE;

    switch (TheEngineState)
    {
    case EEngineStateInit:
        lcd_init ();
        TheEngineState = EEngineStateState1;
        break;
    case EEngineStateState1:
        lcd_clear ();
        TheEngineState = EEngineStateState2;
        break;
    case EEngineStateState2:
        {
        muint8 sh = 7;
        lcd_print_char (0 + sh, 1, 'H');
        lcd_paint_char (8 + sh, 1, 'e');
        lcd_print_char (16 + sh, 1, 'l');
        lcd_paint_char (24 + sh, 1, 'l');
        lcd_print_char (32 + sh, 1, 'o');
        lcd_paint_char (40 + sh, 1, '!');
        }
        TheEngineState = EEngineStateState3;
        break;
    case EEngineStateState3:
        lcd_set_pixel (0, 0, TRUE);
        lcd_set_pixel (0, 15, TRUE);
        lcd_set_pixel (60, 0, TRUE);
        lcd_set_pixel (60, 15, TRUE);
        TheEngineState = EEngineStateState4;
        break;
    case EEngineStateState4:
        lcd_flash ();
        TheEngineState = EEngineStateDone;
        break;
    case EEngineStateDone:
        more = FALSE;
        break;
    };

    return more;
}

#if 0
#include "hw-i2c.h"

static muint8 TheTempBuffer[3] = {
    0x07, 0x10
};

static void m_hw_i2c_write_done (mbool aSuccess, muint8 aBytesWritten) {
    if (aSuccess && 2 == aBytesWritten) {
        const muint8 sh = 2;

        lcd_clear ();
        lcd_print_char (0 + sh, 1, 'X');
        lcd_paint_char (8 + sh, 1, 'X');
        lcd_print_char (16 + sh, 1, 'Y');
        lcd_paint_char (24 + sh, 1, 'Y');
        lcd_print_char (32 + sh, 1, 'Z');
        lcd_paint_char (40 + sh, 1, 'Z');
        lcd_paint_char (48 + sh, 1, '!');
        lcd_flash ();
    }
    else {
        const muint8 sh = 2;

        lcd_clear ();
        lcd_print_char (0 + sh, 1, 'E');
        lcd_paint_char (8 + sh, 1, 'R');
        lcd_print_char (16 + sh, 1, 'R');
        lcd_paint_char (24 + sh, 1, 'O');
        lcd_print_char (32 + sh, 1, 'R');
        lcd_paint_char (40 + sh, 1, 'R');
        lcd_paint_char (48 + sh, 1, '!');
        lcd_flash ();
    }
}
#endif

static void engine_timer_tick (void) {
    static muint16 cnt = 0;

#if 0
    if (0 == cnt) {
        hw_i2c_write (0xD0U, bb, 2, &m_hw_i2c_write_done);
    }
#endif

    if (1000 == cnt) {
        hw_rtc_get_time (&engine_rtc_time_ready);
        cnt = 0;
    }

    cnt++;
}

static void engine_rtc_time_ready (const TRtcTimeInfo *aTimeInfo) {
    static muint8 n = 0;

    m_return_if_fail (aTimeInfo);

    if (0x01U & ++n) {
        /* show time */
        lcd_clear ();
        lcd_print_char (0, 1, '0' + ((aTimeInfo->mHour&0xF0U)>>4));
        lcd_paint_char (8, 1, '0' + ((aTimeInfo->mHour&0x0FU)));
        lcd_print_char (15, 1, ':');
        lcd_paint_char (22, 1, '0' + ((aTimeInfo->mMinute&0xF0U)>>4));
        lcd_print_char (30, 1, '0' + ((aTimeInfo->mMinute&0x0FU)));
        lcd_paint_char (37, 1, ':');
        lcd_paint_char (44, 1, '0' + ((aTimeInfo->mSeconds&0xF0U)>>4));
        lcd_paint_char (52, 1, '0' + ((aTimeInfo->mSeconds&0x0FU)));
        lcd_flash ();
    }
    else {
        /* show data */
        lcd_clear ();
        lcd_print_char (0, 1, 'D');
        lcd_paint_char (8, 1, ':');
        /*lcd_print_char (16, 1, '0');*/
        lcd_paint_char (20, 1, '0' + ((aTimeInfo->mMonth&0xF0U)>>4));
        lcd_print_char (28, 1, '0' + ((aTimeInfo->mMonth&0x0FU)));
        lcd_paint_char (36, 1, '-');
        lcd_paint_char (44, 1, '0' + ((aTimeInfo->mDay&0xF0U)>>4));
        lcd_paint_char (52, 1, '0' + ((aTimeInfo->mDay&0x0FU)));
        lcd_flash ();
    }
}
