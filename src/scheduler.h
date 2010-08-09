#ifndef __AVR_LCD_PROJECT_SCHEDULER_H__
#define __AVR_LCD_PROJECT_SCHEDULER_H__

#include "types.h"

/**
 * The task tick function
 * @param[in] data The user pointer
 * @return The flag if another piece of work is pending
 */
typedef uint8_t (*scheduler_tick) (void *aData);

/**
 * Initialize the task scheduler
 */
void scheduler_init (void);
/**
 * Start the scheduler
 */
void scheduler_start (void);
/**
 * Send request to stop the scheduler
 */
void scheduler_stop (void);
/**
 * Add another item to the scheduler
 * @param[in] aTick The tick function
 * @param[in] aPointer The user pointer to be passed to the tick function
 */
void scheduler_add (scheduler_tick aTick, void *aPointer);
/**
 * Remove the item from the scheduler
 * @param[in] aTick The tick function to be removed from the scheduler
 */
void scheduler_remove (scheduler_tick aTick);

#endif /* __AVR_LCD_PROJECT_SCHEDULER_H__ */
