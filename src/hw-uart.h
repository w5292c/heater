#ifndef __HPRI_PROJECT_HW_UART_H__
#define __HPRI_PROJECT_HW_UART_H__

#include "types.h"

/**
 * The read buffer length
 */
#define M_UART_READ_BUFFER_LENGTH (82)

/**
 * Initialize the UART HW driver
 */
void uart_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the UART HW driver
 */
void uart_deinit (void);
#endif /* !M_NO_DEINIT */

typedef enum {
    EUartStatusSuccess,
    EUartStatusPartial,
    EUartStatusBusy,
    EUartStatusFailed
} TUartStatus;

/**
 * UART reader callback
 * @param[in] aStatus The success status of the UART read operation
 * @param[in] aData The read data
 * @param[in] aCount The number of bytes read from the UART interface
 * @note The callback is invoked in the interrupt context!
 */
typedef void (*uart_reader) (TUartStatus aStatus, const muint8 *aData, muint aCount);
/**
 * UART writer callback
 * @param[in] aStatus The status of the corresponding write operation
 * @note The callback is invoked in the interrupt context!
 */
typedef void (*uart_writer) (TUartStatus aStatus);

/**
 * Regiter UART reader
 */
void uart_register_reader (uart_reader aReader);
/**
 * Write aCount number of bytes located in RAM pointed by aData
 * @param[in] aData The pointer to the data to be written
 * @param[in] aCount The number of bytes to write
 */
void uart_write (uart_writer aWriter, const muint8 *aData, muint aCount);
/**
 * Write aCount number of bytes located in FLASH pointed by aData
 * @param[in] aData The pointer to the data to be written
 * @param[in] aCount The number of bytes to write
 */
void uart_write_p (uart_writer aWriter, const muint8 *aData, muint aCount);

#endif /* __HPRI_PROJECT_HW_UART_H__ */
