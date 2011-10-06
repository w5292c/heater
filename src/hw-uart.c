#include "hw-uart.h"

#include "macros.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

typedef enum {
    EUartStateNull  = 0x00,
    EUartReadIdle   = 0x01,
    EUartReadActive = 0x02,
    EUartWriteIdle  = 0x04,
    EUartWriteRam   = 0x08,
    EUartWriteFlash = 0x10
} TUartState;

static uart_reader TheReader = NULL;
static uart_writer TheWriter = NULL;
static TUartState TheState = EUartStateNull;
static muint8 TheReadBuffer[M_UART_READ_BUFFER_LENGTH];

static muint TheWriterBytesLeft = 0;
static const muint8 *TheWriterPointer = NULL;

void uart_init (void) {
    if (EUartStateNull == TheState) {
        TheState = (EUartReadIdle | EUartWriteIdle);
    }
    (void)TheReadBuffer;
}

#ifndef M_NO_DEINIT
void uart_deinit (void) {
}
#endif /* !M_NO_DEINIT */

void uart_register_reader (uart_reader aReader) {
    m_assert (!TheReader, PSTR("U:r:bsy"));
    TheReader = aReader;
}

void uart_write (uart_writer aWriter, const muint8 *aData, muint aCount) {
    m_assert (!TheWriter, PSTR("U:w:bsy"));
    m_assert (aWriter, PSTR("U:w:arg1"));
    m_assert (aData, PSTR("U:w:arg2"));
    m_assert (aCount, PSTR("U:w:arg3"));

    if ((TheState & EUartWriteRam) || (TheState == EUartWriteFlash)) {
        if (aWriter) {
            (*aWriter)(EUartStatusBusy);
        }
    }
    else {
        TheState = (TheState | EUartWriteRam);

        TheWriter = aWriter;
        TheWriterPointer = aData;
        TheWriterBytesLeft = aCount;
    }
}

void uart_write_p (uart_writer aWriter, const muint8 *aData, muint aCount) {
    m_assert (!TheWriter, PSTR("U:f:bsy"));
    m_assert (aWriter, PSTR("U:f:arg1"));
    m_assert (aData, PSTR("U:f:arg2"));
    m_assert (aCount, PSTR("U:f:arg3"));

    if ((TheState & EUartWriteRam) || (TheState == EUartWriteFlash)) {
        if (aWriter) {
            (*aWriter)(EUartStatusBusy);
        }
    }
    else {
        TheState = (TheState | EUartWriteFlash);

        TheWriter = aWriter;
        TheWriterPointer = aData;
        TheWriterBytesLeft = aCount;
    }
}

ISR (USART_RXC_vect) {
    /* USART, Rx Complete */
}

ISR (USART_UDRE_vect) {
    /* USART Data Register Empty */
}

ISR (USART_TXC_vect) {
    /* USART, Tx Complete */
}
