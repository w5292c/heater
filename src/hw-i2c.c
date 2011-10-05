#include "hw-i2c.h"

#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <util/twi.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

typedef enum {
    /* system states */
    EI2CStateNull = 0,
    EI2CStateIdle,
    EI2CStateCancelling,
    /* RTC reading states */
    EI2CStateRdSendStart,
    EI2CStateRdSendSlvAddr,
    EI2CStateRdReadingBytes,
    EI2CStateRdReadingBytesLast,
    EI2CStateRdDone,
    EI2CStateRdDoneError,
    /* RTC writing states */
    EI2CStateWrSendStart,
    EI2CStateWrSendSlvAddr,
    EI2CStateWrWritingData,
    EI2CStateWrDone,
    EI2CStateWrDoneError
} TI2CState;

typedef struct {
    muint8 mAddress;
    muint8 mRequestLenght;
    union {
        struct {
            muint8 *mBuffer;
            hw_i2c_read_done mCallback;
        } mRead;
        struct {
            const muint8 *mBuffer;
            hw_i2c_write_done mCallback;
        } mWrite;
    } mData;
} TClientRequest;

/**
 * The data represinting the current request
 */
static TClientRequest TheRequest;

static muint8 volatile TheI2CIndex = 0;
static muint8 volatile TheI2CState = EI2CStateNull;

/**
 * The scheduler tick
 * @return TRUE if more work is already available
 */
static mbool hw_i2c_sched_tick (void);
/**
 * Send 'start' I2C protocol condition
 */
static inline void hw_i2c_send_start (void);

void hw_i2c_init (void) {
    TheI2CState = EI2CStateIdle;

    scheduler_add (&hw_i2c_sched_tick);
    TWBR = 0x0CU;
    TWDR = 0xFFU;
    TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
    PORTC |= (muint8)((1<<PC0) | (1<<PC1));
}

#ifndef M_NO_DEINIT
void hw_i2c_deinit (void) {
    scheduler_remove (&hw_i2c_sched_tick);
}
#endif /* !M_NO_DEINIT */

void hw_i2c_read (muint8 aAddr, muint8 *aBuff, muint8 aSize, hw_i2c_read_done aCallback) {
    m_return_if_fail (aCallback);

    if (EI2CStateIdle == TheI2CState) {
        TheRequest.mAddress = (aAddr & 0xFEU);
        TheRequest.mRequestLenght = aSize;
        TheRequest.mData.mRead.mBuffer = aBuff;
        TheRequest.mData.mRead.mCallback = aCallback;

        TheI2CState = EI2CStateRdSendStart;
        hw_i2c_send_start ();
    }
    else {
        (*aCallback) (FALSE, 0);
    }
}

void hw_i2c_write (muint8 aAddr, const muint8 *aBuff, muint8 aSize, hw_i2c_write_done aCallback) {
    m_return_if_fail (aCallback);

    if (EI2CStateIdle == TheI2CState) {
        TheRequest.mAddress = (aAddr & 0xFEU);
        TheRequest.mRequestLenght = aSize;
        TheRequest.mData.mWrite.mBuffer = aBuff;
        TheRequest.mData.mWrite.mCallback = aCallback;

        TheI2CState = EI2CStateWrSendStart;
        hw_i2c_send_start ();
    }
    else {
        (*aCallback) (FALSE, 0);
    }
}

static mbool hw_i2c_sched_tick (void) {
    switch (TheI2CState)
    {
    case EI2CStateWrDone:
        TheI2CState = EI2CStateIdle;
        (*TheRequest.mData.mWrite.mCallback) (TRUE, TheRequest.mRequestLenght);
        break;
    case EI2CStateWrDoneError:
        TheI2CState = EI2CStateIdle;
        (*TheRequest.mData.mWrite.mCallback) (FALSE, 0);
        break;
    case EI2CStateRdDone:
        TheI2CState = EI2CStateIdle;
        (*TheRequest.mData.mRead.mCallback) (TRUE, TheRequest.mRequestLenght);
        break;
    case EI2CStateRdDoneError:
        (*TheRequest.mData.mRead.mCallback) (FALSE, 0);
        break;
    default:
        break;
    }

    return FALSE;
}

static inline void hw_i2c_send_start (void) {
    /* send the START condition */
    TWCR = ((1<<TWINT) | (1<<TWSTA) | (1<<TWEN)) | (1<<TWIE);
}

/**
 * Handle the event that the START condition is sent
 */
static inline void hw_i2c_handle_start_transmitted (void) {
    switch (TheI2CState)
    {
    case EI2CStateWrSendStart:
    case EI2CStateRdSendStart:
        if (EI2CStateWrSendStart == TheI2CState) {
            TheI2CState = EI2CStateWrSendSlvAddr;
            /* send address (write) request */
            TWDR = (TheRequest.mAddress | TW_WRITE);
        }
        else /* if (EI2CStateRdSendStart == TheI2CState) */ {
            TheI2CState = EI2CStateRdSendSlvAddr;
            /* send address (read) request */
            TWDR = (TheRequest.mAddress | TW_READ);
        }
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
    case EI2CStateRdSendSlvAddr:
        TheI2CIndex = 0;
        if (1 != TheRequest.mRequestLenght) {
            /* more than 1 byte to receive, send ACK */
            TheI2CState = EI2CStateRdReadingBytes;
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        }
        else {
            TheI2CState = EI2CStateRdReadingBytesLast;
            /* only 1 byte to receive, send NACK */
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        }
        break;
    case EI2CStateWrSendSlvAddr:
        TheI2CIndex = 0;
        TheI2CState = EI2CStateWrWritingData;
        TWDR = TheRequest.mData.mWrite.mBuffer[0];
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
    case EI2CStateWrWritingData:
        if (TheRequest.mRequestLenght != (TheI2CIndex + 1)) {
            /* not all the data transmitted, send the next byte */
            ++TheI2CIndex;
            TWDR = TheRequest.mData.mWrite.mBuffer[TheI2CIndex];
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        }
        else {
            /* all the requested data sent, send the stop condition */
            TheI2CState = EI2CStateWrDone;
            TWCR = (1<<TWEN)|(0<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|(0<<TWWC);
        }
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
        TheRequest.mData.mRead.mBuffer[TheI2CIndex] = data;
        if ((TheRequest.mRequestLenght - 1) == ++TheI2CIndex) {
            /* request reading the last byte */
            /* TWI Interface enabled */
            TheI2CState = EI2CStateRdReadingBytesLast;
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        }
        else {
            /* request reading another byte */
            /* TWI Interface enabled */
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
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
        TheRequest.mData.mRead.mBuffer[TheI2CIndex] = data;
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
    const muint8 data = TW_STATUS;

    switch (data)
    {
    case TW_START:
    case TW_REP_START:
        hw_i2c_handle_start_transmitted ();
        break;
    case TW_MT_SLA_ACK:
        hw_i2c_handle_slave_address_transmitted ();
        break;
    case TW_MR_SLA_ACK:
        hw_i2c_handle_slave_address_transmitted ();
        break;
    case TW_MT_DATA_ACK:
        hw_i2c_handle_data_transmitted ();
        break;
    case TW_MR_DATA_ACK:
    case TW_SR_DATA_ACK:
        hw_i2c_handle_data_received_ack ();
        break;
    case TW_SR_DATA_NACK:
    case TW_MR_DATA_NACK:
        hw_i2c_handle_data_received_nack ();
        break;
    }
}
