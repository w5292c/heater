#ifndef __AVR_LCD_PROJECT_ENGINE_IDLE_H__
#define __AVR_LCD_PROJECT_ENGINE_IDLE_H__

#include "types.h"

/**
 * Initialize the Idle engine state
 */
void engine_state_idle_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the Idle engine state
 */
void engine_state_idle_deinit (void);
#endif /* !M_NO_DEINIT */

#endif /* __AVR_LCD_PROJECT_ENGINE_IDLE_H__ */
