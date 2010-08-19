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
