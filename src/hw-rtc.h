#ifndef __AVR_LCD_PROJECT_HW_RTC_H__
#define __AVR_LCD_PROJECT_HW_RTC_H__

#include "types.h"

typedef struct {
    /* the time information */
    uint8_t mSeconds;
    uint8_t mMinute;
    uint8_t mHour;
    /* the date information */
    uint8_t mDay;
    uint8_t mMonth;
    uint8_t mYear;
} TRtcTimeInfo;

typedef void (*hw_rtc_time_ready) (TRtcTimeInfo *aTimeInfo);

void hw_rtc_init (void);
void hw_rtc_deinit (void);

void hw_rtc_get_time (hw_rtc_time_ready aCallback);

#endif /* __AVR_LCD_PROJECT_HW_RTC_H__ */
