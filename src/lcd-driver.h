#ifndef __AVR_LCD_PROJECT_LCD_DRIVER_H__
#define __AVR_LCD_PROJECT_LCD_DRIVER_H__

#include "types.h"

void lcd_init (void);
void lcd_deinit (void);

void lcd_flash (void);
void lcd_clear (void);
void lcd_set_pixel (muint8 aX, muint8 aY, muint8 aValue);
void lcd_print_char (muint8 aX, muint8 aY, muint8 aChar);
void lcd_paint_char (muint8 aX, muint8 aY, muint8 aChar);

#endif /* __AVR_LCD_PROJECT_LCD_DRIVER_H__ */
