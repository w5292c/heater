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

#include "hw-keys.h"

#include "macros.h"
#include "hw-timer.h"

#ifdef M_AVR
#include <avr/io.h>
#endif /* M_AVR */

static hw_keys_on_key_event TheCallbacks[2] = {NULL};

/**
 * The handler for a 1ms timeout event
 */
static void hw_keys_timer_callback (void);

void hw_keys_init (void) {
    hw_timer_add_callback (&hw_keys_timer_callback);
}

#ifndef M_NO_DEINIT
void hw_keys_deinit (void) {
    hw_timer_remove_callback (&hw_keys_timer_callback);
}
#endif /* !M_NO_DEINIT */

void hw_keys_add_key_event_handler (hw_keys_on_key_event aCallback) {
    muint8 i;
    for (i = 0; i < 2; i++) {
        if (!TheCallbacks[i]) {
            TheCallbacks[i] = aCallback;
            break;
        }
    }
}

void hw_keys_remove_key_event_handler (hw_keys_on_key_event aCallback) {
    muint8 i;
    for (i = 0; i < 2; i++) {
        if (TheCallbacks[i] == aCallback) {
            TheCallbacks[i] = NULL;
            break;
        }
    }
}

/**
 * Here is how key events are handled:
 * Key is pressed:
 * - 10 periods: The 1st key event;
 * - 250 periods: The 1st key event;
 * - Every 100 extra periods: key event;
 */
static void hw_keys_handle_key_pressed (muint8 aKey, mbool aPressed) {
    /**
     * This static array contains information about key presses:
     * 0 - Key is not pressed;
     * 1..9 - Key is pessed, client is not informaed;
     * 10 - Inform the client;
     * 11..249 - Delay before the repitition;
     * 250 - Inorm the 2nd time;
     * 251..350 - Repetition delay;
     * 351 - Set 250 and inform the client;
     */
    static muint16 TheKeysInfo[EHwKeyCodeKeyCount] = {0};

    /* check that aKey is correct */
    m_return_if_fail (aKey < EHwKeyCodeKeyCount);

    if (aPressed) {
        muint16 count;

        count = ++TheKeysInfo[aKey];

        if (351 == count) {
            count = 250;
            TheKeysInfo[aKey] = 250;
        }
        if (10 == count || 250 ==count) {
            muint8 i;

            for (i = 0; i < 2; i++) {
                if (TheCallbacks[i]) {
                    (*TheCallbacks[i]) (aKey);
                }
            }
        }
    }
    else {
        TheKeysInfo[aKey] = 0;
    }
}

/* handle joystick events */
static inline void hw_keys_handle_keys_1 (muint8 aScanCode) {
    hw_keys_handle_key_pressed (EHwKeyCodeKeyUp, 0x06U == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyDown, 0x09U == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyLeft, 0x0CU == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyRight, 0x03U == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyUpLeft, 0x0EU == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyUpRight, 0x07U == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyDownLeft, 0x0DU == aScanCode);
    hw_keys_handle_key_pressed (EHwKeyCodeKeyDownRight, 0x0BU == aScanCode);
}

/* handle numeric events: 1..4 */
static inline void hw_keys_handle_keys_2 (muint8 aScanCode) {
    hw_keys_handle_key_pressed (EHwKeyCodeKey1, !(aScanCode & 0x01U));
    hw_keys_handle_key_pressed (EHwKeyCodeKey2, !(aScanCode & 0x02U));
    hw_keys_handle_key_pressed (EHwKeyCodeKey3, !(aScanCode & 0x04U));
    hw_keys_handle_key_pressed (EHwKeyCodeKey4, !(aScanCode & 0x08U));
}

/* handle 2 extra keys: center joystick key and the numeric key '5' */
static inline void hw_keys_handle_keys_3 (muint8 aScanCode) {
    hw_keys_handle_key_pressed (EHwKeyCodeKey5, !(aScanCode & 0x01U));
    /*hw_keys_handle_key_pressed (EHwKeyCodeKeyCenter, !(aScanCode & 0x02U));*/
}

static void hw_keys_timer_callback (void) {
#ifdef M_AVR
    /* set up pull-up resitors for keyboard input pins */
    DDRA = 0;
    PORTA |= (1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3);
    /* select the 1st row */
    DDRD = (1<<DDD4)|(1<<DDD5)|(1<<DDD6);
    PORTD = (1<<PD3)| /* pull-up for expernal interrupt */
        (1<<PD4)|(1<<PD5); /* activate the row 1 */
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);

    /* now read the keys */
    const muint8 data = PINA&0x0FU;
    hw_keys_handle_keys_1 (data);

    PORTD = (1<<PD3)| /* pull-up for expernal interrupt */
        (1<<PD4)|(1<<PD6); /* activate the row 2 */
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);

    /* now read the keys */
    hw_keys_handle_keys_2 (PINA&0x0FU);

    PORTD = (1<<PD3)| /* pull-up for expernal interrupt */
        (1<<PD5)|(1<<PD6); /* activate the row 3 */
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);

    /* now read the keys */
    hw_keys_handle_keys_3 (PINA&0x0FU);

    /* restore default state for the ports */
    DDRA = 0;
    PORTA = 0;
    DDRD = 0;
    PORTD = (1<<PD3);
    /* Sync + 200 ns delay */
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
    asm volatile ("nop\n" ::);
#endif /* M_AVR */
}
