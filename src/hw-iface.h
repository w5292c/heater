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

#ifndef __AVR_LCD_PROJECT_HW_IFACE_H__
#define __AVR_LCD_PROJECT_HW_IFACE_H__

#include "types.h"

/**
 * Initialize the HW interface
 */
void hw_init (void);
/**
 * Read data byte in the LCD module
 * @return The read data byte
 */
muint8 hw_read_data (void);
/**
 * Read the status byte of the LCD module
 * @return The status byte of the LCD module
 */
muint8 hw_read_status (void);
/**
 * Write a command byte to the LCD module
 * @param[in] aByte The command byte to be written to the LCD module
 */
void hw_write_cmd (muint8 aCmd);
/**
 * Write a data byte to the LCD module
 * @param[in] aByte The data byte to be written to the LCD module
 */
void hw_write_data (muint8 aData);
/**
 * Set the 'reset' pin for the LCD module
 */
void hw_reset_lcd (void);

#endif /* __AVR_LCD_PROJECT_HW_IFACE_H__ */
