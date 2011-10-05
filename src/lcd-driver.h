#ifndef __AVR_LCD_PROJECT_LCD_DRIVER_H__
#define __AVR_LCD_PROJECT_LCD_DRIVER_H__

#include "types.h"

void lcd_init (void);
#ifndef M_NO_DEINIT
void lcd_deinit (void);
#endif /* !M_NO_DEINIT */

void lcd_flash (void);
void lcd_clear (void);
void lcd_set_pixel (muint8 aX, muint8 aY, muint8 aValue);
void lcd_print_char_7x14 (muint8 aX, muint8 aY, muint8 aChar);
void lcd_paint_char_7x14 (muint8 aX, muint8 aY, muint8 aChar);
void lcd_paint_string_7x14 (muint8 aX, muint8 aY, const mchar *aString);
void lcd_paint_string_7x14_p (muint8 aX, muint8 aY, const mchar *aString);

void lcd_print_char_5x8 (muint8 aX, muint8 aY, muint8 aChar);
void lcd_paint_char_5x8 (muint8 aX, muint8 aY, muint8 aChar);
void lcd_paint_string_5x8 (muint8 aX, muint8 aY, const mchar *aString);
void lcd_paint_string_5x8_p (muint8 aX, muint8 aY, const mchar *aString);

#endif /* __AVR_LCD_PROJECT_LCD_DRIVER_H__ */
