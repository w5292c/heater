#ifndef __AVR_LCD_PROJECT_ENGINE_H__
#define __AVR_LCD_PROJECT_ENGINE_H__

#include "hw-rtc.h"

typedef enum {
    EEngineStateIdle = 0,
    EEngineStateTimeSet,
    EEngineStateDateSet,
    EEngineStateControl,
    EEngineStateLast,
    EEngineStateNull = EEngineStateLast
} TEngineState;

typedef void (*engine_state_enter) (void);
typedef void (*engine_state_leave) (void);
typedef void (*engine_state_tick) (void);
typedef void (*engine_state_timer) (void);
typedef void (*engine_state_rtc_timer) (void);
typedef void (*engine_state_key_event) (muint8 aCode);

typedef struct {
    engine_state_enter mEnter;
    engine_state_leave mLeave;
    engine_state_tick mTick;
    engine_state_timer mTimer;
    engine_state_rtc_timer mRtcTimer;
    engine_state_key_event mKeyEvent;
} TEngineStateInterface;

/**
 * Initialize the engine
 */
void engine_init (void);
/**
 * Deinitialize the engine
 */
void engine_deinit (void);
/**
 * Register anoter state
 */
void engine_register_state (muint8 aEngineState, TEngineStateInterface *aInterface);
/**
 * Request state transition
 */
void engine_request_state (muint8 aNewState);
/**
 * Get the current time
 */
const TRtcTimeInfo *engine_get_current_time (void);

#endif /* __AVR_LCD_PROJECT_ENGINE_H__ */
