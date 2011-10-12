#ifndef __HPRI_PROJECT_AT_CMD_ENGINE_H__
#define __HPRI_PROJECT_AT_CMD_ENGINE_H__

#include "types.h"

void at_engine_init (void);
#ifndef M_NO_DEINIT
/**
 * Deinitialize the UART HW driver
 */
void at_engine_deinit (void);
#endif /* !M_NO_DEINIT */

#endif /* __HPRI_PROJECT_HW_UART_H__ */
