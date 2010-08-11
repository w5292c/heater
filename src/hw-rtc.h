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
/**
 * The client callback to notify that the time info has been written to the RTC
 * @param[in] mSuccess Success of the operation
 */
typedef void (*hw_rtc_time_written) (mbool mSuccess);

/**
 * Initialize the RTC system
 */
void hw_rtc_init (void);
/**
 * Deinitialize the RTC system
 */
void hw_rtc_deinit (void);

/**
 * Start request to get time information
 * @param[in] aCallback The client callback to notify that the operation has been finished
 */
void hw_rtc_get_time (hw_rtc_time_ready aCallback);
/**
 * Start request to set time information
 * @param[in aTime The time information to be set
 * @param[in] aCallback The client callback to notify that the operation has been finished
 */
void hw_rtc_set_time (const TRtcTimeInfo *aTime, hw_rtc_time_written aCallback);

#endif /* __AVR_LCD_PROJECT_HW_RTC_H__ */
