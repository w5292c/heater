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

#ifndef __AVR_LCD_PROJECT_HW_TIMER_H__
#define __AVR_LCD_PROJECT_HW_TIMER_H__

#include "types.h"

#define M_MAX_TIMER_TICKS (3U)

typedef void (*hw_timer_callback) (void);

/**
 * Initalize the timer HW
 */
void hw_timer_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the timer HW
 */
void hw_timer_deinit (void);
#endif /* !M_NO_DEINIT */

void hw_timer_add_callback (hw_timer_callback aCallback);
void hw_timer_remove_callback (hw_timer_callback aCallback);

#endif /* __AVR_LCD_PROJECT_HW_TIMER_H__ */
