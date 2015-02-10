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

#ifndef __AVR_LCD_PROJECT_FONTS_H__
#define __AVR_LCD_PROJECT_FONTS_H__

#include "types.h"

#if 0
#define M_FONTS_ENABLE_EXTENDED
#endif

/**
 * Get the image for 5X8 font
 * @param[in] aChar The required character code
 * @param[in] aLine The line of image [0..7]
 * @return The image for the corresponding line of character
 */
muint8 get_font_5x8_byte (muint8 aChar, muint8 aLine);
/**
 * Get the image for 7X14 font
 * @param[in] aChar The required character code
 * @param[in] aLine The line of image [0..13]
 * @return The image for the corresponding line of character
 */
muint8 get_font_7x14_byte (muint8 aChar, muint8 aLine);

#endif /* __AVR_LCD_PROJECT_FONTS_H__ */
