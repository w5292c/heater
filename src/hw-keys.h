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

#ifndef __AVR_LCD_PROJECT_HW_KEYS_H__
#define __AVR_LCD_PROJECT_HW_KEYS_H__

#include "types.h"

typedef enum {
    EHwKeyCodeKey1,
    EHwKeyCodeKey2,
    EHwKeyCodeKey3,
    EHwKeyCodeKey4,
    EHwKeyCodeKey5,
    EHwKeyCodeKeyUp,
    EHwKeyCodeKeyDown,
    EHwKeyCodeKeyLeft,
    EHwKeyCodeKeyRight,
    EHwKeyCodeKeyUpLeft,
    EHwKeyCodeKeyUpRight,
    EHwKeyCodeKeyDownLeft,
    EHwKeyCodeKeyDownRight,
    EHwKeyCodeKeyCenter,
    /* the number of key codes */
    EHwKeyCodeKeyCount,
    /* special codes */
    EHwKeyCodeNone
} THwKeyCode;

/**
 * Callback for the key event
 * @param[in] aCode The code defined by 'THwKeyCode'
 */
typedef void (*hw_keys_on_key_event) (muint8 aCode);

/**
 * Initialize the keyboard system
 */
void hw_keys_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the keyboard system
 */
void hw_keys_deinit (void);
#endif /* !M_NO_DEINIT */
/**
 * Add a callback for a key event
 */
void hw_keys_add_key_event_handler (hw_keys_on_key_event aCallback);
/**
 * Remove a callback for a key event
 */
void hw_keys_remove_key_event_handler (hw_keys_on_key_event aCallback);

#endif /* __AVR_LCD_PROJECT_HW_KEYS_H__ */
