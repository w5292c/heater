#ifndef __AVR_LCD_PROJECT_SCHEDULER_H__
#define __AVR_LCD_PROJECT_SCHEDULER_H__

#include "types.h"

/**
 * The task tick function
 * @param[in] data The user pointer
 * @return The flag if another piece of work is pending
 */
typedef mbool (*scheduler_tick) (void);

/**
 * Initialize the task scheduler
 */
void scheduler_init (void);
/**
 * Start the scheduler
 */
void scheduler_start (void);
#ifndef M_NO_DEINIT
/**
 * Send request to stop the scheduler
 */
void scheduler_stop (void);
#endif /* !M_NO_DEINIT */
/**
 * Add another item to the scheduler
 * @param[in] aTick The tick function
 */
void scheduler_add (scheduler_tick aTick);
/**
 * Remove the item from the scheduler
 * @param[in] aTick The tick function to be removed from the scheduler
 */
void scheduler_remove (scheduler_tick aTick);
/**
 * Enter the sleep mode
 */
void scheduler_enter_sleep_mode (void);
/**
 * Leave the sleep mode
 */
void scheduler_leave_sleep_mode (void);

#endif /* __AVR_LCD_PROJECT_SCHEDULER_H__ */
