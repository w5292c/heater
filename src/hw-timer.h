#ifndef __AVR_LCD_PROJECT_HW_TIMER_H__
#define __AVR_LCD_PROJECT_HW_TIMER_H__

#include "types.h"

#define M_MAX_TIMER_TICKS (3U)

typedef void (*hw_timer_callback) (void);

/**
 * Initalize the timer HW
 */
void hw_timer_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the timer HW
 */
void hw_timer_deinit (void);
#endif /* !M_NO_DEINIT */

void hw_timer_add_callback (hw_timer_callback aCallback);
void hw_timer_remove_callback (hw_timer_callback aCallback);

#endif /* __AVR_LCD_PROJECT_HW_TIMER_H__ */
