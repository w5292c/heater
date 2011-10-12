#include "hw-uart.h"

#include "macros.h"
#include "scheduler.h"

#ifdef M_AVR
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#endif /* M_AVR */

typedef enum {
    EUartStateNull  = 0x00,
    EUartWriteRam   = 0x01,
    EUartWriteFlash = 0x02,
} TUartState;

static uart_reader TheReader = NULL;
static uart_writer TheWriter = NULL;
static volatile TUartState TheState = EUartStateNull;
static muint8 TheReadBuffer[M_UART_READ_BUFFER_LENGTH];
static volatile muint8 TheReaderBytesRead = 0;

static volatile muint TheWriterBytesLeft = 0;
static volatile const muint8 *TheWriterPointer = NULL;

void uart_init (void) {
    m_assert (EUartStateNull == TheState, PSTR("U:w-st"));
    UBRRH = 0;
    UBRRL = 16;
    UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<TXCIE);
    UCSRA = (1<<U2X);
    /* Set frame format: 8 data bits, 1 stop bit */
    UCSRC = (1<<URSEL)|(3<<UCSZ0);

    TheState = EUartStateNull;
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
    m_assert (aData, PSTR("U:w:arg2"));
    m_assert (aCount, PSTR("U:w:arg3"));

    if ((TheState & EUartWriteRam) || (TheState & EUartWriteFlash)) {
        if (aWriter) {
            (*aWriter)(EUartStatusBusy);
        }
        return;
    }
    else {
        TheState = (TheState | EUartWriteRam);

        TheWriter = aWriter;
        TheWriterPointer = aData;
        TheWriterBytesLeft = aCount;

        UDR = *TheWriterPointer;
        ++TheWriterPointer;
        --TheWriterBytesLeft;
    }
}

void uart_write_p (uart_writer aWriter, const muint8 *aData, muint aCount) {
    m_assert (aData, PSTR("U:f:arg2"));
    m_assert (aCount, PSTR("U:f:arg3"));

    if ((TheState & EUartWriteRam) || (TheState & EUartWriteFlash)) {
        if (aWriter) {
            (*aWriter)(EUartStatusBusy);
        }
    }
    else {
        TheState |= EUartWriteFlash;

        TheWriter = aWriter;
        TheWriterPointer = aData;
        TheWriterBytesLeft = aCount;

        UDR = pgm_read_byte (TheWriterPointer);
        ++TheWriterPointer;
        --TheWriterBytesLeft;
    }
}

ISR (USART_RXC_vect) {
    /* USART, Rx Complete */
    const muint8 data = UDR;
    if (data == 0x0D || data == 0x0A) {
        /* the end-of-line marker received */
        TheReadBuffer[TheReaderBytesRead] = 0x00;
        if (TheReader) {
            (*TheReader) (EUartStatusSuccess, TheReadBuffer, TheReaderBytesRead);
        }
        TheReaderBytesRead = 0;
    }
    else {
        TheReadBuffer[TheReaderBytesRead] = data;
        ++TheReaderBytesRead;
        m_assert (TheReaderBytesRead < M_UART_READ_BUFFER_LENGTH - 1, PSTR("toolong"));
    }
}

ISR (USART_UDRE_vect) {
    /* USART Data Register Empty */
    m_assert(0, PSTR("UDRE"));
}

ISR (USART_TXC_vect) {
    /* USART, Tx Complete */
    if (EUartWriteRam & TheState) {
        if (TheWriterBytesLeft) {
            m_assert(TheWriterPointer, PSTR("U:noptr"));
            UDR = *TheWriterPointer;
            ++TheWriterPointer;
            --TheWriterBytesLeft;
        }
        else {
            TheState &= ~EUartWriteRam;
            /* we have just written the last byte*/
            TheWriterPointer = NULL;
            /* already done: TheWriterBytesLeft = 0; */
            if (TheWriter) {
                (*TheWriter) (EUartStatusSuccess);
            }
        }
    }
    else if (EUartWriteFlash & TheState) {
        if (TheWriterBytesLeft) {
            m_assert(TheWriterPointer, PSTR("U:noptr"));
            UDR = pgm_read_byte (TheWriterPointer);
            ++TheWriterPointer;
            --TheWriterBytesLeft;
        }
        else {
            TheState &= ~EUartWriteFlash;
            /* we have just written the last byte*/
            TheWriterPointer = NULL;
            /* already done: TheWriterBytesLeft = 0; */
            if (TheWriter) {
                (*TheWriter) (EUartStatusSuccess);
            }
        }
    }
    else {
        m_assert (0, PSTR("wrong"));
    }
}
