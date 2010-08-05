#ifndef __AVR_LCD_PROJECT_LCD_DRIVER_H__
#define __AVR_LCD_PROJECT_LCD_DRIVER_H__

#include "types.h"

void lcd_init (void);
void lcd_deinit (void);

void lcd_flash (void);
void lcd_clear (void);
void lcd_set_pixel (uint8_t aX, uint8_t aY, uint8_t aValue);
void lcd_print_char (uint8_t aX, uint8_t aY, uint8_t aChar);

void lcd_debug_show_buffer (void);

#endif /* __AVR_LCD_PROJECT_LCD_DRIVER_H__ */