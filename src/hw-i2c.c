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

static muint8 TheI2CIndex = 0;
static muint8 TheI2CState = EI2CStateNull;

/*void hw_i2c_read (hw_i2c_read_done aCallback);
void hw_i2c_write  (hw_i2c_write_done aCallback);*/

/**
 * The current 'read' request callback
 */
static hw_i2c_read_done TheReadCallback;
/**
 * The current 'write' request callback
 */
static hw_i2c_write_done TheWriteCallback;

/**
 * The scheduler tick
 * @return TRUE if more work is already available
 */
static mbool hw_i2c_sched_tick (void);
/**
 * Start reading RTC using I2C protocol
 */
static inline void hw_i2c_start_read (void);

void hw_i2c_init (void) {
    TheReadCallback = NULL;
    TheWriteCallback = NULL;
    TheI2CState = EI2CStateIdle;

    scheduler_add (&hw_i2c_sched_tick);
}

void hw_i2c_deinit (void) {
    scheduler_remove (&hw_i2c_sched_tick);

    TheReadCallback = NULL;
    TheWriteCallback = NULL;
}

void hw_i2c_read (muint8 aAddr, muint8 *aBuff, muint8 aSize, hw_i2c_read_done aCallback) {
    m_return_if_fail (aCallback);

    if (EI2CStateIdle == TheI2CState) {
        TheRequest.mAddress = (aAddr & 0xFEU);
        TheRequest.mRequestLenght = aSize;
        TheRequest.mData.mRead.mBuffer = aBuff;
        TheRequest.mData.mRead.mCallback = aCallback;
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
    }
    else {
        (*aCallback) (FALSE, 0);
    }
}

static mbool hw_i2c_sched_tick (void) {
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
        TWDR = (TheRequest.mAddress | TW_WRITE);
        /* TWI Interface enabled, Enable TWI Interupt and clear the flag to send byte */
        TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
        break;
    case EI2CStateRdSendStart2:
        TheI2CState = EI2CStateRdSendSlvRdAddr;
        /* send address (read) request */
        TWDR = (TheRequest.mAddress | TW_READ);
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
        if ((TheRequest.mRequestLenght - 1) == ++TheI2CIndex) {
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
