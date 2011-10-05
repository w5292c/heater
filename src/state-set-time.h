#ifndef __AVR_LCD_PROJECT_STATE_SET_TIME_H__
#define __AVR_LCD_PROJECT_STATE_SET_TIME_H__

#include "types.h"

/**
 * Initialize the Date engine state
 */
void engine_state_set_time_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the Date engine state
 */
void engine_state_set_time_deinit (void);
#endif /* !M_NO_DEINIT */

#endif /* __AVR_LCD_PROJECT_STATE_SET_TIME_H__ */
