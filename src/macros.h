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

#ifndef __AVR_LCD_PROJECT_MACROS_H__
#define __AVR_LCD_PROJECT_MACROS_H__

#include "engine.h"

#define m_assert(arg, desc) arg || (engine_panic_p (desc),1);

#ifdef M_DEBUG_LOGGING
#ifdef M_PC
#include <stdio.h>
#endif

#define m_return_if_fail(arg) \
    if (!(arg)) { \
        m_assert (arg, PSTR("m_return_if_fail")); \
        return; \
    }
#define m_return_val_if_fail(arg,val) \
    if (!(arg)) { \
        m_assert (arg, PSTR ("m_return_val_if_fail")); \
        return val; \
    }
#else
#define m_return_if_fail(arg) \
    if (!(arg)) { \
        return; \
    }
#define m_return_val_if_fail(arg,val) \
    if (!(arg)) { \
        return val; \
    }
#endif /* M_DEBUG_LOGGING */

#ifdef M_PC
#define M_FLASH
#else
#define M_FLASH PROGMEM
#endif

#define TRUE (1)
#define FALSE (0)
#define NULL ((void *)0)

#define M_UNUSED_PARAM(param) (void)param

#ifdef M_PC
/**
 * Dummy PC definitions
 */
#include <unistd.h>
#define _delay_us(arg) usleep (arg)
#define _delay_ms(arg) usleep (arg*1000)
#define pgm_read_byte(arg) (*((mchar*)arg))
#define PSTR(arg) (arg)
#endif
#ifdef M_AVR
/**
 * Dummy AVR definitions
 */
#define printf(...)
#define printf(...)
#endif

#endif /* __AVR_LCD_PROJECT_MACROS_H__ */
