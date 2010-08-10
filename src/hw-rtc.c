#include "hw-rtc.h"

#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <string.h>
#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

/**
 * Preliminary estimation that A0 is '0'
 */
#define M_RTC_ADDRESS (0xA2U)
/**
 * The number of butes to be read
 */
#define M_RTC_BYTE_TO_READ (0x05)

typedef enum {
    /* system states */
    EI2CStateIdle,
    EI2CStateCancelling,
    /* RTC reading states */
    EI2CStateRdSendStart1,
    EI2CStateRdSendSlvWrAddr,
    EI2CStateRdSendRamAddr,
    EI2CStateRdSendStart2,
    EI2CStateRdSendSlvRdAddr,
    EI2CStateRdReadingBytes,
    EI2CStateRdReadingBytesLast,
    EI2CStateRdDone,
    EI2CStateRdDoneError,
    /* RTC writing states */
    EI2CStateWr,
    EI2CStateWrDone,
    EI2CStateWrDoneError
} TI2CState;

static TRtcTimeInfo TheTimeInfo;
static muint8 volatile TheI2CState;
static muint8 volatile TheI2CIndex;
static hw_rtc_time_ready TheCallback;
/**
 * The tack to report to the client when RTC data is ready
 * @return FALSE No more work is needed
 */
static muint8 hw_rtc_tick (void);
/**
 * Start reading RTC using I2C protocol
 */
static inline void hw_i2c_start_read (void);

void hw_rtc_init (void) {
    TheCallback = NULL;
    TheI2CState = EI2CStateIdle;
    memset (&TheTimeInfo, 0, sizeof (TRtcTimeInfo));

    scheduler_add (&hw_rtc_tick);
}

void hw_rtc_deinit (void) {
    scheduler_remove (&hw_rtc_tick);

    TheCallback = NULL;
    TheI2CState = EI2CStateCancelling;
}

void hw_rtc_get_time (hw_rtc_time_ready aCallback) {
    m_return_if_fail (TheCallback);
    m_return_if_fail (EI2CStateIdle == TheI2CState);

    TheCallback = aCallback;
    memset (&TheTimeInfo, 0, sizeof (TRtcTimeInfo));

    hw_i2c_start_read ();
}

static muint8 hw_rtc_tick (void) {
    if (EI2CStateRdDone == TheI2CState || EI2CStateRdDoneError == TheI2CState) {
        /* the time is ready, inform the client */
        m_return_val_if_fail (TheCallback, FALSE);

        (*TheCallback) (&TheTimeInfo);
        TheCallback = NULL;
        TheI2CState = EI2CStateIdle;
    }
    else if (EI2CStateWrDone == TheI2CState || EI2CStateWrDoneError == TheI2CState) {
        /* the time has been written, inform the client */
        /** @todo implement */
        TheI2CState = EI2CStateIdle;
    }

    return FALSE;
}

static inline void hw_i2c_start_read (void) {
    /* send the START condition */
    TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
    TheI2CState = EI2CStateRdSendStart1;
}

/**
 * Handle the event that the START condition is sent
 */
static inline void hw_i2c_handle_start_transmitted (void) {
    switch (TheI2CState)
    {
    case EI2CStateRdSendStart1:
        TheI2CState = EI2CStateRdSendSlvWrAddr;
        /* send address (write) request */
        TWDR = (M_RTC_ADDRESS | TW_WRITE);
        /* TWI Interface enabled, Enable TWI Interupt and clear the flag to send byte */
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        break;
    case EI2CStateRdSendStart2:
        TheI2CState = EI2CStateRdSendSlvRdAddr;
        /* send address (read) request */
        TWDR = (M_RTC_ADDRESS | TW_READ);
        /* TWI Interface enabled, Enable TWI Interupt and clear the flag to send byte */
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        break;
    default:
        /** @todo implement error handling */
        m_return_if_fail (FALSE);
        break;
    }
}

static inline void hw_i2c_handle_slave_address_transmitted (void) {
    switch (TheI2CState)
    {
    case EI2CStateRdSendSlvWrAddr:
        TheI2CState = EI2CStateRdSendRamAddr;
        /* read starting the 2st byte (seconds) */
        TWDR = 0x02U;
        TWCR = (1<<TWINT)|(1<<TWEN);
        break;
    case EI2CStateRdSendSlvRdAddr:
        TheI2CState = EI2CStateRdReadingBytes;
        TheI2CIndex = 0;
        /* TWI Interface enabled;
           Enable TWI Interupt and clear the flag to read next byte;
           Send ACK after reception; */
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        break;
    default:
        /** @todo implement error handling */
        m_return_if_fail (FALSE);
        break;
    }
}

static inline void hw_i2c_handle_data_transmitted (void) {
    switch (TheI2CState)
    {
    case EI2CStateRdSendRamAddr:
        TheI2CState = EI2CStateRdSendStart2;
        /* send the START condition */
        TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN));
        break;
    default:
        /** @todo implement error handling */
        m_return_if_fail (FALSE);
        break;
    }
}

static inline void hw_i2c_handle_data_received_ack (void) {
    muint8 data;

    switch (TheI2CState)
    {
    case EI2CStateRdReadingBytes:
        data = TWDR;
        switch (TheI2CIndex)
        {
        case 0:
            TheTimeInfo.mSeconds = data;
            break;
        case 1:
            TheTimeInfo.mMinute = data;
            break;
        case 2:
            TheTimeInfo.mHour = data;
            break;
        case 3:
            TheTimeInfo.mDay = data;
            TheTimeInfo.mYear = data;
            break;
        default:
            m_return_if_fail (FALSE && TheI2CIndex);
            break;
        }
        if ((M_RTC_BYTE_TO_READ - 1) == ++TheI2CIndex) {
            /* request reading the last byte */
            /* TWI Interface enabled */
            TheI2CState = EI2CStateRdReadingBytesLast;
            TWCR = (1<<TWEN)|
                /* Enable TWI Interupt and clear the flag to read next byte */
                (1<<TWIE)|(1<<TWINT)|
                /* Send NACK after reception */
                (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|
                (0<<TWWC);
        }
        else {
            /* request reading another byte */
            /* TWI Interface enabled */
            TWCR = (1<<TWEN)|
                /* Enable TWI Interupt and clear the flag to read next byte */
                (1<<TWIE)|(1<<TWINT)|
                /* Send ACK after reception */
                (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|
                (0<<TWWC);
        }
        break;
    default:
        /** @todo implement error handling */
        m_return_if_fail (FALSE);
        break;
    }
}

static inline void hw_i2c_handle_data_received_nack (void) {
    muint8 data;

    switch (TheI2CState)
    {
    case EI2CStateRdReadingBytesLast:
        data = TWDR;
        TheTimeInfo.mMonth = data;
        /* TWI Interface enabled;
           Disable TWI Interrupt and clear the flag;
           Initiate a STOP condition. */
        TheI2CState = EI2CStateRdDone;
        TWCR = (1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);
        break;
    default:
        /** @todo implement error handling */
        m_return_if_fail (FALSE);
        break;
    }
}

ISR(TWI_vect) {
    switch (TW_STATUS)
    {
    case TW_START:
    case TW_REP_START:
        hw_i2c_handle_start_transmitted ();
        break;
    case TW_MT_SLA_ACK:
        hw_i2c_handle_slave_address_transmitted ();
        break;
    case TW_MT_DATA_ACK:
        hw_i2c_handle_data_transmitted ();
        break;
    case TW_MR_DATA_ACK:
        hw_i2c_handle_data_received_ack ();
        break;
    case TW_MR_DATA_NACK:
        hw_i2c_handle_data_received_nack ();
        break;
    }
}
