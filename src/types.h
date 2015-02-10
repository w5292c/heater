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

#ifndef __AVR_LCD_PROJECT_TYPES_H__
#define __AVR_LCD_PROJECT_TYPES_H__

#include <inttypes.h>

/**
 * 8-bit types
 */
typedef char mchar;
typedef uint8_t mbool;
typedef int8_t mint8;
typedef uint8_t muint8;
/**
 * 16-bit types
 */
typedef int16_t mint16;
typedef uint16_t muint16;
/**
 * 32 bit types
 */
typedef int32_t mint32;
typedef uint32_t muint32;
/**
 * Platform-natural 'int' types
 */
typedef mint16 mint;
typedef muint16 muint;
typedef muint16 msize;

#endif /* __AVR_LCD_PROJECT_TYPES_H__ */
