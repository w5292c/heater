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

#ifndef __AVR_LCD_PROJECT_SCHEDULER_H__
#define __AVR_LCD_PROJECT_SCHEDULER_H__

#include "types.h"

/**
 * The task tick function
 * @param[in] data The user pointer
 * @return The flag if another piece of work is pending
 */
typedef mbool (*scheduler_tick) (void);

/**
 * Initialize the task scheduler
 */
void scheduler_init (void);
/**
 * Start the scheduler
 */
void scheduler_start (void);
#ifndef M_NO_DEINIT
/**
 * Send request to stop the scheduler
 */
void scheduler_stop (void);
#endif /* !M_NO_DEINIT */
/**
 * Add another item to the scheduler
 * @param[in] aTick The tick function
 */
void scheduler_add (scheduler_tick aTick);
/**
 * Remove the item from the scheduler
 * @param[in] aTick The tick function to be removed from the scheduler
 */
void scheduler_remove (scheduler_tick aTick);
/**
 * Enter the sleep mode
 */
void scheduler_enter_sleep_mode (void);
/**
 * Leave the sleep mode
 */
void scheduler_leave_sleep_mode (void);

#endif /* __AVR_LCD_PROJECT_SCHEDULER_H__ */
