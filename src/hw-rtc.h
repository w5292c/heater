#ifndef __AVR_LCD_PROJECT_HW_RTC_H__
#define __AVR_LCD_PROJECT_HW_RTC_H__

#include "types.h"

typedef void (*hw_rtc_time_ready) (void);

void hw_rtc_init (void);
void hw_rtc_deinit (void);

void hw_rtc_get_time (hw_rtc_time_ready aCallback);

#endif /* __AVR_LCD_PROJECT_HW_RTC_H__ */
