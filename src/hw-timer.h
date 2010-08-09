#ifndef __AVR_LCD_PROJECT_HW_TIMER_H__
#define __AVR_LCD_PROJECT_HW_TIMER_H__

#include "types.h"

#define M_MAX_TIMER_TICKS (1U)

typedef void (*hw_timer_tick) (void);

void hw_timer_init (void);
void hw_timer_deinit (void);

void hw_timer_add_tick (hw_timer_tick aTimerTick);
void hw_timer_remove_tick (hw_timer_tick aTimerTick);

#endif /* __AVR_LCD_PROJECT_HW_TIMER_H__ */
