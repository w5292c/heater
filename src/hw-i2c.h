#ifndef __AVR_LCD_PROJECT_HW_I2C_H__
#define __AVR_LCD_PROJECT_HW_I2C_H__

#include "types.h"

typedef void (*hw_i2c_time_ready) (void);

void hw_i2c_init (void);
void hw_i2c_deinit (void);

void hw_i2c_get_time (hw_i2c_time_ready aCallback);

#endif /* __AVR_LCD_PROJECT_HW_I2C_H__ */
