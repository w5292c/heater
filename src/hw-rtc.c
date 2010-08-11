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
    EHwRtcStateWrWritingAddressData,
} THwRtcState;

/**
 * Preliminary estimation that A0 is '0'
 */
#define M_RTC_ADDRESS (0xA2U)
/**
 * The number of butes to be read
 */
#define M_RTC_BYTE_TO_READ (0x05)
#define M_RTC_READ_BUFFER_LENGHT (0x08)

static muint8 TheRtcState = EHwRtcStateNull;
static TRtcTimeInfo TheTimeInfo;
static muint8 volatile TheI2CState;
static hw_rtc_time_ready TheCallback;
static muint8 TheTimeReadBuffer[M_RTC_READ_BUFFER_LENGHT];

static void hw_rtc_i2c_read_done (mbool aSuccess, muint8 aBytesRead);
static void hw_rtc_i2c_write_done (mbool aSuccess, muint8 aBytesWritten);

void hw_rtc_init (void) {
    m_return_if_fail (EHwRtcStateNull == TheRtcState);

    TheCallback = NULL;
    TheRtcState = EHwRtcStateIdle;
    memset (&TheTimeInfo, 0, sizeof (TRtcTimeInfo));

    hw_i2c_init ();
}

void hw_rtc_deinit (void) {
    hw_i2c_deinit ();

    TheCallback = NULL;
    TheI2CState = EHwRtcStateCancel;
}

void hw_rtc_get_time (hw_rtc_time_ready aCallback) {
    static const muint8 DataAddress[] = {
        0x00, 0x00
    };

    m_return_if_fail (TheCallback);
    m_return_if_fail (EHwRtcStateIdle == TheI2CState);

    TheCallback = aCallback;
    memset (&TheTimeInfo, 0, sizeof (TRtcTimeInfo));

    TheI2CState = EHwRtcStateRdWritingAddress;
    hw_i2c_write (M_RTC_ADDRESS, DataAddress, 2, &hw_rtc_i2c_write_done);
}

static inline void hw_rtc_handle_i2c_rd_addr_written (mbool aSuccess, muint8 aBytesWritten) {
    if (aSuccess && 2 == aBytesWritten) {
        TheI2CState = EHwRtcStateRdReadingData;
        hw_i2c_read (M_RTC_ADDRESS,
            TheTimeReadBuffer, M_RTC_READ_BUFFER_LENGHT, hw_rtc_i2c_read_done);
    }
    else {
        hw_rtc_time_ready callback;

        /* error happened, reset the state */
        callback = TheCallback;
        TheCallback = NULL;
        TheI2CState = EHwRtcStateIdle;
        m_return_if_fail (callback);

        /* notify the client */
        (*callback) (NULL);
    }
}

static void hw_rtc_i2c_write_done (mbool aSuccess, muint8 aBytesWritten) {
    switch (TheRtcState)
    {
    case EHwRtcStateRdWritingAddress:
        hw_rtc_handle_i2c_rd_addr_written (aSuccess, aBytesWritten);
        break;
    case EHwRtcStateRdReadingData:
    case EHwRtcStateWrWritingAddressData:
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
    TheI2CState = EHwRtcStateIdle;
    callback = TheCallback;
    TheCallback = NULL;

    if (aSuccess && M_RTC_READ_BUFFER_LENGHT == aBytesRead) {
        /** @todo implement parsing the read buffer */

        /* notify the client */
        m_return_if_fail (callback);
        (*callback) (&TheTimeInfo);
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
    case EHwRtcStateWrWritingAddressData:
    case EHwRtcStateRdWritingAddress:
    case EHwRtcStateNull:
    case EHwRtcStateIdle:
    case EHwRtcStateCancel:
        hw_rtc_handle_i2c_rd_data_ready (FALSE, 0);
        break;
    }
}
