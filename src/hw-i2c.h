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
/**
 * Deinitialize the I2C system
 */
void hw_i2c_deinit (void);
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
