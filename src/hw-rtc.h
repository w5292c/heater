/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Alexander Chumakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
typedef void (*hw_rtc_time_ready) (const TRtcTimeInfo *aTimeInfo);
/**
 * The client callback to notify that the time info has been written to the RTC
 * @param[in] mSuccess Success of the operation
 */
typedef void (*hw_rtc_time_written) (mbool mSuccess);
/**
 * The 1 second RTC callback
 */
typedef void (*hw_rtc_timer) (void);

/**
 * Initialize the RTC system
 */
void hw_rtc_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the RTC system
 */
void hw_rtc_deinit (void);
#endif /* !M_NO_DEINIT */

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
/**
 * Set the callback for the RTC timer
 * @param[in] aCallback The callback to be set
 */
void hw_rtc_set_rtc_timer (hw_rtc_timer aCallback);

#endif /* __AVR_LCD_PROJECT_HW_RTC_H__ */
