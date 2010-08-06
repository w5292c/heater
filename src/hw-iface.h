#ifndef __AVR_LCD_PROJECT_HW_IFACE_H__
#define __AVR_LCD_PROJECT_HW_IFACE_H__

#include "types.h"

typedef enum {
    EHwFlagsReset,
    EHwFlagsCommand,
    EHwFlagsA0,
    EHwFlagsRdWr
} THwIFaceExtraFlags;

uint8_t hw_read_data (void);
uint8_t hw_read_status (void);
void hw_write_cmd (uint8_t aCmd);
void hw_write_data (uint8_t aData);
void hw_set_bit (THwIFaceExtraFlags aBit);
void hw_reset_bit (THwIFaceExtraFlags aBit);

#endif /* __AVR_LCD_PROJECT_HW_IFACE_H__ */
