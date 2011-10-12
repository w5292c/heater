#include "at-cmd-engine.h"

#include "macros.h"
#include "hw-uart.h"

#ifdef M_AVR
#include <string.h>
#include <avr/pgmspace.h>
#endif /* M_AVR */

static void at_engine_uart_reader (TUartStatus aStatus, const muint8 *aData, muint aCount);

void at_engine_init (void) {
    uart_init ();
    uart_register_reader (at_engine_uart_reader);
}

#ifndef M_NO_DEINIT
void at_engine_deinit (void) {
    uart_deinit ();
}
#endif /* !M_NO_DEINIT */

static mbool b = FALSE;
static muint8 buf[90];
static void at_engine_uart_writer (TUartStatus aStatus) {
    m_assert (EUartStatusSuccess == aStatus, PSTR("e:wstt"));

    if (!b) {
        uart_write (at_engine_uart_writer, buf, strlen ((const char *)buf));
        b = TRUE;
    }
    else {
        uart_write_p (NULL, (const muint8 *)PSTR("\r\n"), 2);
        b = FALSE;
    }
}

void at_engine_uart_reader (TUartStatus aStatus, const muint8 *aData, muint aCount) {
    (void)aData;
    (void)aCount;
    if (EUartStatusSuccess == aStatus) {
        m_assert (aCount < 85, PSTR("long rsp"));
        if (aCount) {
            memcpy (buf, aData, aCount);
            buf[aCount] = 0;
        }
        else {
            memcpy_P (buf, PSTR("<null>"), 7);
        }
        uart_write_p (at_engine_uart_writer, (const unsigned char*)PSTR("pre:"), 4);
    }
}
