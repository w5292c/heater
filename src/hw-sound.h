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

#ifndef __AVR_LCD_PROJECT_HW_SOUND_H__
#define __AVR_LCD_PROJECT_HW_SOUND_H__

#include "types.h"

/**
 * The callback function to notify that the note is finished
 */
typedef void (*hw_sound_finished) (void);

/**
 * Initialize the sound hardware
 */
void hw_sound_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the sound hardware
 */
void hw_sound_deinit (void);
#endif /* !M_NO_DEINIT */

/**
 * Play a note:
 * --------------------    ----------------------
 * |  # | Note | Code |    |  # | Octave | Code |
 * --------------------    ----------------------
 * |  1 |   C  | 0x00 |    |  1 |     0  | ---- |
 * |  2 |   C# | 0x01 |    |  2 |     1  | ---- |
 * |  3 |   D  | 0x02 |    |  3 |     2  | ---- |
 * |  4 |   D# | 0x03 |    |  4 |     3  | ---- |
 * |  5 |   E  | 0x04 |    |  5 |    (4) | 0x40 |
 * |  6 |   F  | 0x05 |    |  6 |     5  | 0x50 |
 * |  7 |   F# | 0x06 |    |  7 |     6  | 0x60 |
 * |  8 |   G  | 0x07 |    |  8 |     7  | 0x70 |
 * |  9 |   G# | 0x08 |    |  9 |     8  | 0x80 |
 * | 10 |  (A) | 0x09 |    ----------------------
 * | 11 |   B  | 0x0A |
 * | 12 |   H  | 0x0B |
 * --------------------
 * @param[in] aNote The Note and Octave information
 * @param[in] aLength The length of the note in msec
 * @param[in] aCallback The callback when the note is finished
 * @note Special value for the 'aNote' 0xFFU means 'pause' for aLength ms
 */
void hw_sound_play_note (muint8 aNote, muint16 aLength, hw_sound_finished aCallback);

#endif /* __AVR_LCD_PROJECT_HW_SOUND_H__ */
