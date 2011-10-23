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
