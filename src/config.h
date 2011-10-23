#ifndef __AVR_LCD_PROJECT_CONFIG_H__
#define __AVR_LCD_PROJECT_CONFIG_H__

#ifdef M_PC
/**
 * Uncomment this to enable debug logging
 */
#define M_DEBUG_LOGGING
#endif
/**
 * The maximum number of scheduler tasks
 */
#define M_MAX_TICKS (4U)
/**
 * This configuration setting enables programming fuse bits
 */
#if 0
#define CONFIG_PROGRAM_FUSES
#endif

#endif/* __AVR_LCD_PROJECT_CONFIG_H__ */
