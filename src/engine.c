#include "engine.h"

#include "macros.h"
#include "hw-timer.h"
#include "scheduler.h"
#include "lcd-driver.h"

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
static muint8 engine_tick (void);
/**
 * 1 KHz timer tick
 */
static void engine_timer_tick (void);

void engine_init (void) {
    scheduler_add (&engine_tick);
    hw_timer_init ();
    hw_timer_add_callback (&engine_timer_tick);
}

void engine_deinit (void) {
    hw_timer_remove_callback (&engine_timer_tick);
    hw_timer_deinit ();
    scheduler_remove (&engine_tick);

    /* deinitialize the LCD interface */
    lcd_deinit ();
}

static muint8 engine_tick (void) {
    muint8 more = TRUE;

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

static void engine_timer_tick (void) {
    static muint16 cnt = 0;

    if (1000 == cnt) {
        const muint8 sh = 2;

        lcd_clear ();
        lcd_print_char (0 + sh, 1, 'A');
        lcd_paint_char (8 + sh, 1, 'n');
        lcd_print_char (16 + sh, 1, 'o');
        lcd_paint_char (24 + sh, 1, 't');
        lcd_print_char (32 + sh, 1, 'h');
        lcd_paint_char (40 + sh, 1, 'e');
        lcd_paint_char (48 + sh, 1, 'r');
        lcd_set_pixel (0, 0, TRUE);
        lcd_set_pixel (0, 15, TRUE);
        lcd_set_pixel (60, 0, TRUE);
        lcd_set_pixel (60, 15, TRUE);
        lcd_flash ();
    }
    else if (2000 == cnt) {
        const muint8 sh = 7;

        lcd_clear ();
        lcd_print_char (0 + sh, 1, 'H');
        lcd_paint_char (8 + sh, 1, 'e');
        lcd_print_char (16 + sh, 1, 'l');
        lcd_paint_char (24 + sh, 1, 'l');
        lcd_print_char (32 + sh, 1, 'o');
        lcd_paint_char (40 + sh, 1, '!');
        lcd_flash ();

        /* reset the static counter */
        cnt = 0;
    }

    cnt++;
}
