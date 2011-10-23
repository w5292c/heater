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

#ifndef __AVR_LCD_PROJECT_ENGINE_H__
#define __AVR_LCD_PROJECT_ENGINE_H__

#include "hw-rtc.h"

typedef enum {
    EEngineStateIdle = 0,
    EEngineStateDate,
    EEngineStateAlarm,
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
#ifndef M_NO_DEINIT
/**
 * Deinitialize the engine
 */
void engine_deinit (void);
#endif /* !M_NO_DEINIT */
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
/**
 * Panic the engine
 * @param[in] aPanicInfo The string in the flash memory describing the panic reason
 * @note This function does not exit, it suspends the CPU
 */
void engine_panic_p (const char *aPanicInfo);

#endif /* __AVR_LCD_PROJECT_ENGINE_H__ */
