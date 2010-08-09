#include "engine.h"

#include "macros.h"
#include "scheduler.h"
#include "lcd-driver.h"

typedef enum {
    EEngineStateInit,
    EEngineStateState1,
    EEngineStateState2,
    EEngineStateState3,
    EEngineStateState4,
    EEngineStateState5,
    EEngineStateDone
} TEngineState;

static TEngineState TheEngineState = EEngineStateInit;

/**
 * The task scheduler tick for the engine
 * @param[in] aData The engine pointer
 * @return More work is pending
 */
static uint8_t engine_tick (void);

void engine_init (void) {
    scheduler_add (&engine_tick);
}

static uint8_t engine_tick (void) {
    uint8_t more = TRUE;

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
        uint8_t sh = 7;
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
        TheEngineState = EEngineStateState5;
        break;
    case EEngineStateState5:
        lcd_deinit ();
        TheEngineState = EEngineStateDone;
        break;
    case EEngineStateDone:
        more = FALSE;
        break;
    };

    return more;
}
