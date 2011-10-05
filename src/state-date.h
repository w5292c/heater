#ifndef __AVR_LCD_PROJECT_ENGINE_DATE_H__
#define __AVR_LCD_PROJECT_ENGINE_DATE_H__

#include "types.h"

/**
 * Initialize the Date engine state
 */
void engine_state_date_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the Date engine state
 */
void engine_state_date_deinit (void);
#endif /* !M_NO_DEINIT */

#endif /* __AVR_LCD_PROJECT_ENGINE_DATE_H__ */
