#ifndef __AVR_LCD_PROJECT_HW_RTC_H__
#define __AVR_LCD_PROJECT_HW_RTC_H__

#include "types.h"

typedef struct {
    /* the time information */
    muint8 mCentiSeconds;
    muint8 mSeconds;
    muint8 mMinute;
    muint8 mHour;
    /* the date information */
    muint8 mDay;
    muint8 mMonth;
    muint16 mYear;
} TRtcTimeInfo;

/**
 * The client callback to notify that another timer info is retrieved
 * @param[in] aTimeInfo The retrieved time infomation
 */
typedef void (*hw_rtc_time_ready) (TRtcTimeInfo *aTimeInfo);

void hw_rtc_init (void);
void hw_rtc_deinit (void);

void hw_rtc_get_time (hw_rtc_time_ready aCallback);

#endif /* __AVR_LCD_PROJECT_HW_RTC_H__ */
