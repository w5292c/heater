#ifndef __AVR_LCD_PROJECT_TYPES_H__
#define __AVR_LCD_PROJECT_TYPES_H__

#include <inttypes.h>

/**
 * 8-bit types
 */
typedef char mchar;
typedef uint8_t mbool;
typedef int8_t mint8;
typedef uint8_t muint8;
/**
 * 16-bit types
 */
typedef int16_t mint16;
typedef uint16_t muint16;
/**
 * 32 bit types
 */
typedef int32_t mint32;
typedef uint32_t muint32;
/**
 * Platform-natural 'int' types
 */
typedef mint16 mint;
typedef muint16 muint;
typedef muint16 msize;

#endif /* __AVR_LCD_PROJECT_TYPES_H__ */
