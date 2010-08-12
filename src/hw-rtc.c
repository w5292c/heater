#include "hw-rtc.h"

#include "hw-i2c.h"
#include "macros.h"

#ifdef M_AVR
#include <string.h>
#include <avr/pgmspace.h>
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
    EHwRtcStateWrWritingAddress,
    EHwRtcStateWrWritingData,
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
static union {
    hw_rtc_time_ready mRdFunc;
    hw_rtc_time_written mWrFunc;
} TheCallback;
static muint8 TheBuffer[M_RTC_READ_BUFFER_LENGHT];
static const muint8 TheRtcDataAddressStart[] M_FLASH = {
    0x00
};

static void hw_rtc_i2c_read_done (mbool aSuccess, muint8 aBytesRead);
static void hw_rtc_i2c_write_done (mbool aSuccess, muint8 aBytesWritten);

void hw_rtc_init (void) {
    m_return_if_fail (EHwRtcStateNull == TheRtcState);

    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateIdle;

    hw_i2c_init ();
}

void hw_rtc_deinit (void) {
    hw_i2c_deinit ();

    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateCancel;
}

void hw_rtc_get_time (hw_rtc_time_ready aCallback) {
    m_return_if_fail (aCallback);
    m_return_if_fail (EHwRtcStateIdle == TheRtcState);

    TheCallback.mRdFunc = aCallback;
    memcpy_P (TheBuffer, TheRtcDataAddressStart, 1);

    TheRtcState = EHwRtcStateRdWritingAddress;
    hw_i2c_write (M_RTC_ADDRESS, TheBuffer, 1, &hw_rtc_i2c_write_done);
}

void hw_rtc_set_time (const TRtcTimeInfo *aTime, hw_rtc_time_written aCallback) {
    m_return_if_fail (aTime);
    m_return_if_fail (aCallback);
    m_return_if_fail (EHwRtcStateIdle == TheRtcState);

    TheCallback.mWrFunc = aCallback;
    memcpy_P (TheBuffer, TheRtcDataAddressStart, 1);

    TheRtcState = EHwRtcStateWrWritingAddress;
    hw_i2c_write (M_RTC_ADDRESS, TheBuffer, 1, &hw_rtc_i2c_write_done);
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

static inline void hw_rtc_handle_i2c_wr_addr_written (mbool aSuccess, muint8 aBytesWritten) {
    if (aSuccess && 1 == aBytesWritten) {
        TheRtcState = EHwRtcStateWrWritingData;
        /** prepare TheBuffer for the write request */
        hw_i2c_write (M_RTC_ADDRESS,
            TheBuffer, M_RTC_READ_BUFFER_LENGHT, hw_rtc_i2c_write_done);
    }
    else {
        hw_rtc_time_written callback;

        /* error happened, reset the state */
        callback = TheCallback.mWrFunc;
        TheCallback.mRdFunc = NULL;
        TheRtcState = EHwRtcStateIdle;

        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (FALSE);
    }
}

static inline void hw_rtc_handle_i2c_wr_data_written (mbool aSuccess, muint8 aBytesWritten) {
    hw_rtc_time_written callback;

    callback = TheCallback.mWrFunc;
    TheCallback.mRdFunc = NULL;
    TheRtcState = EHwRtcStateIdle;

    if (aSuccess && 7 == aBytesWritten) {
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
    case EHwRtcStateWrWritingAddress:
        hw_rtc_handle_i2c_wr_addr_written (aSuccess, aBytesWritten);
        break;
    case EHwRtcStateWrWritingData:
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
    case EHwRtcStateWrWritingAddress:
    case EHwRtcStateWrWritingData:
    case EHwRtcStateRdWritingAddress:
    case EHwRtcStateNull:
    case EHwRtcStateIdle:
    case EHwRtcStateCancel:
        hw_rtc_handle_i2c_rd_data_ready (FALSE, 0);
        break;
    }
}
