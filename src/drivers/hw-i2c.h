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

#ifndef __AVR_LCD_PROJECT_HW_I2C_H__
#define __AVR_LCD_PROJECT_HW_I2C_H__

#include "types.h"

/**
 * The client callback to notify that read request finished
 * @param[in] aSuccess Success of the read operation
 * @param[in] aBytesRead The number of bytes read
 */
typedef void (*hw_i2c_read_done) (mbool aSuccess, muint8 aBytesRead);
/**
 * The client callback to notify that write request finished
 * @param[in] aSuccess Success of the write operation
 * @param[in] aBytesRead The number of bytes written
 */
typedef void (*hw_i2c_write_done) (mbool aSuccess, muint8 aBytesWritten);

/**
 * Initialize the I2C system
 */
void hw_i2c_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the I2C system
 */
void hw_i2c_deinit (void);
#endif /* !M_NO_DEINIT */
/**
 * Start the read request
 * @param[in] aAddr The address of the I2C client to be read
 * @param[in] aBuff The read buffer
 * @param[in] aSize The number of bytes to read
 * @note There can only be 1 outstanding read or write request at any given time
 */
void hw_i2c_read (muint8 aAddr, muint8 *aBuff, muint8 aSize, hw_i2c_read_done aCallback);
/**
 * Start the write request
 * @param[in] aAddr
 * @param[in] aBuff
 * @param[in] aSize
 * @note There can only be 1 outstanding read or write request at any given time
 */
void hw_i2c_write (muint8 aAddr, const muint8 *aBuff, muint8 aSize, hw_i2c_write_done aCallback);

#endif /* __AVR_LCD_PROJECT_HW_I2C_H__ */
