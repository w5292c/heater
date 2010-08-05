#ifndef __AVR_LCD_PROJECT_MACROS_H__
#define __AVR_LCD_PROJECT_MACROS_H__

#ifdef M_DEBUG_LOGGING
#include <stdio.h>

#define m_return_if_fail(arg) \
    if (!(arg)) { \
        fprintf (stderr, "Warning: function: %s; failed: [%s]\n", __func__, #arg); \
        return; \
    }
#define m_return_val_if_fail(arg,val) \
    if (!(arg)) { \
        fprintf (stderr, "Warning: function: %s; failed: [%s]\n", __func__, #arg); \
        return val; \
    }
#else
#define m_return_if_fail(arg) \
    if (!(arg)) { \
        return; \
    }
#define m_return_val_if_fail(arg,val) \
    if (!(arg)) { \
        return val; \
    }
#endif /* M_DEBUG_LOGGING */ \

#ifdef M_PC
#define M_FLASH
#else
#define M_FLASH PROGMEM
#endif

#define TRUE (1)
#define FALSE (0)

#define M_UNUSED_PARAM(param) (void)param

#ifdef M_PC
/**
 * Dummy definitions
 */
 #include <unistd.h>
#define _delay_us(arg) usleep (arg)
#define _delay_ms(arg) usleep (arg*1000)
#endif

#endif /* __AVR_LCD_PROJECT_MACROS_H__ */
