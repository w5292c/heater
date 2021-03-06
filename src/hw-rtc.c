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

#include "hw-rtc.h"

#include "hw-i2c.h"
#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <string.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

/**
 * States for RTC read/write requests:
 *
 *
 * RTC Read requests:
 *        [read-time]                  [done]               [done]
 * (Idle) ----------> (WritingAddress) -----> (ReadingData) -----> (Ready)--
 *    ^     [done]                              [sync-with-scheduler]      |
 *    |------------------ ( InformClient ) <--------------------------------
 *
 *
 * RTC Write requests:
 *        [write-time]                                  [done]
 * (Idle) -----------> (WritingAddress and WritingData) -----> (Ready)--
 *    ^     [done]                             [sync-with-scheduler]   |
 *    |------------------ ( InformClient ) <----------------------------
 */

typedef enum {
    /* service states */
    EHwRtcStateNull = 0,
    EHwRtcStateIdle,
    EHwRtcStateCancel,
    /* read states */
    EHwRtcStateRdWritingAddress,
    EHwRtcStateRdReadingData,
    /* write states */
    EHwRtcStateWrWriting,
} THwRtcState;

/**
 * Preliminary estimation that A0 is '0'
 */
#define M_RTC_ADDRESS (0xD0U)
/**
 * The number of butes to be read
 */
#define M_RTC_BYTE_TO_READ (0x05)
#define M_RTC_READ_BUFFER_LENGHT (0x08)

static muint8 TheRtcState = EHwRtcStateNull;
static mbool volatile TheRtcEvent = FALSE;
static hw_rtc_timer TheRtcCallback = NULL;

static union {
    hw_rtc_time_ready mRdFunc;
    hw_rtc_time_written mWrFunc;
} TheCallback;
static muint8 TheBuffer[M_RTC_READ_BUFFER_LENGHT];

static void hw_rtc_i2c_read_done (mbool aSuccess, muint8 aBytesRead);
static void hw_rtc_i2c_write_done (mbool aSuccess, muint8 aBytesWritten);
/**
 * The scheduler tick
 * @return TRUE if more work is already available
 */
static mbool hw_rtc_sched_tick (void);

void hw_rtc_init (void) {
    m_return_if_fail (EHwRtcStateNull == TheRtcState);

    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateIdle;
    scheduler_add (&hw_rtc_sched_tick);

    hw_i2c_init ();
    MCUCR = (muint8)(1<<ISC11) | (1<<ISC10);
    GICR |= (muint8)(1<<INT1);
}

#ifndef M_NO_DEINIT
void hw_rtc_deinit (void) {
    hw_i2c_deinit ();

    scheduler_remove (&hw_rtc_sched_tick);
    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateCancel;
}
#endif /* !M_NO_DEINIT */

void hw_rtc_get_time (hw_rtc_time_ready aCallback) {
    m_return_if_fail (aCallback);
    m_return_if_fail (EHwRtcStateIdle == TheRtcState);

    TheCallback.mRdFunc = aCallback;
    TheBuffer[0] = 0;

    TheRtcState = EHwRtcStateRdWritingAddress;
    hw_i2c_write (M_RTC_ADDRESS, TheBuffer, 1, &hw_rtc_i2c_write_done);
}

void hw_rtc_set_time (const TRtcTimeInfo *aTime, hw_rtc_time_written aCallback) {
    m_return_if_fail (aTime);
    m_return_if_fail (aCallback);
    m_return_if_fail (EHwRtcStateIdle == TheRtcState);

    TheCallback.mWrFunc = aCallback;
    /* Start address: 0x00 */
    TheBuffer[0] = 0;
    /* store the time to be set */
    TheBuffer[1] = aTime->mSeconds; /**< Seconds */
    TheBuffer[2] = aTime->mMinute;  /**< Minutes */
    TheBuffer[3] = aTime->mHour;    /**< Hours */
#if 0 /**! Yet to be implemented */
    TheBuffer[4] = ; /**< Day */
    TheBuffer[5] = ; /**< Date */
    TheBuffer[6] = ; /**< Month */
    TheBuffer[7] = ; /**< Year */
#endif

    TheRtcState = EHwRtcStateWrWriting;
    hw_i2c_write (M_RTC_ADDRESS, TheBuffer, 4, &hw_rtc_i2c_write_done);
}

static inline void hw_rtc_handle_i2c_rd_addr_written (mbool aSuccess, muint8 aBytesWritten) {
    if (aSuccess && 1 == aBytesWritten) {
        TheRtcState = EHwRtcStateRdReadingData;
        memset (TheBuffer, 0, M_RTC_READ_BUFFER_LENGHT);
        hw_i2c_read (M_RTC_ADDRESS,
            TheBuffer, M_RTC_READ_BUFFER_LENGHT, hw_rtc_i2c_read_done);
    }
    else {
        hw_rtc_time_ready callback;

        /* error happened, reset the state */
        callback = TheCallback.mRdFunc;
        TheCallback.mRdFunc = NULL;
        TheRtcState = EHwRtcStateIdle;
        m_return_if_fail (callback);

        /* notify the client */
        (*callback) (NULL);
    }
}

static inline void hw_rtc_handle_i2c_wr_data_written (mbool aSuccess, muint8 aBytesWritten) {
    hw_rtc_time_written callback;

    callback = TheCallback.mWrFunc;
    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateIdle;

    if (aSuccess && 4 == aBytesWritten) {
        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (TRUE);
    }
    else {
        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (FALSE);
    }
}

static void hw_rtc_i2c_write_done (mbool aSuccess, muint8 aBytesWritten) {
    switch (TheRtcState)
    {
    case EHwRtcStateRdWritingAddress:
        hw_rtc_handle_i2c_rd_addr_written (aSuccess, aBytesWritten);
        break;
    case EHwRtcStateWrWriting:
        hw_rtc_handle_i2c_wr_data_written (aSuccess, aBytesWritten);
        break;
    case EHwRtcStateRdReadingData:
    case EHwRtcStateNull:
    case EHwRtcStateIdle:
    case EHwRtcStateCancel:
        hw_rtc_handle_i2c_rd_addr_written (FALSE, 0);
        break;
    }
}

static inline void hw_rtc_handle_i2c_rd_data_ready (mbool aSuccess, muint8 aBytesRead) {
    hw_rtc_time_ready callback;

    /* error happened, reset the state */
    TheRtcState = EHwRtcStateIdle;
    callback = TheCallback.mRdFunc;
    TheCallback.mRdFunc = NULL;

    if (aSuccess && M_RTC_READ_BUFFER_LENGHT == aBytesRead) {
        TRtcTimeInfo info;

        info.mCentiSeconds = 0;
        info.mSeconds = (0x7FU&TheBuffer[0]);
        info.mMinute = (0x7FU&TheBuffer[1]);
        info.mHour = (0x3FU&TheBuffer[2]);
        /* the date information */
        info.mDay = (0x3FU&TheBuffer[4]);
        info.mMonth = (0x1FU&TheBuffer[5]);
        info.mYear = TheBuffer[6];

        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (&info);
    }
    else {
        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (NULL);
    }
}

static void hw_rtc_i2c_read_done (mbool aSuccess, muint8 aBytesRead) {
    switch (TheRtcState)
    {
    case EHwRtcStateRdReadingData:
        hw_rtc_handle_i2c_rd_data_ready (aSuccess, aBytesRead);
        break;
    case EHwRtcStateWrWriting:
    case EHwRtcStateRdWritingAddress:
    case EHwRtcStateNull:
    case EHwRtcStateIdle:
    case EHwRtcStateCancel:
        hw_rtc_handle_i2c_rd_data_ready (FALSE, 0);
        break;
    }
}

void hw_rtc_set_rtc_timer (hw_rtc_timer aCallback) {
    TheRtcCallback = aCallback;
}

static mbool hw_rtc_sched_tick (void) {
    if (TheRtcEvent) {
        /* now we can inform the client */
        if (TheRtcCallback) {
            (*TheRtcCallback) ();
        }

        /* reset the flag */
        TheRtcEvent = FALSE;
    }

    return FALSE;
}

ISR (INT1_vect) {
    TheRtcEvent = TRUE;
}
