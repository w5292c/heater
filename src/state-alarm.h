#ifndef __AVR_LCD_PROJECT_ENGINE_ALARM_H__
#define __AVR_LCD_PROJECT_ENGINE_ALARM_H__

#include "types.h"

/**
 * Initialize the Alarm engine state
 */
void engine_state_alarm_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the Alarm engine state
 */
void engine_state_alarm_deinit (void);
#endif /* !M_NO_DEINIT */

#endif /* __AVR_LCD_PROJECT_ENGINE_ALARM_H__ */
