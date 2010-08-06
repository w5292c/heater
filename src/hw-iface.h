#ifndef __AVR_LCD_PROJECT_HW_IFACE_H__
#define __AVR_LCD_PROJECT_HW_IFACE_H__

#include "types.h"

/**
 * Initialize the HW interface
 */
void hw_init (void);

typedef enum {
    EHwFlagsReset,
    EHwFlagsCommand,
    EHwFlagsA0,
    EHwFlagsRdWr
} THwIFaceExtraFlags;

/**
 * Read data byte in the LCD module
 * @return The read data byte
 */
uint8_t hw_read_data (void);
/**
 * Read the status byte of the LCD module
 * @return The status byte of the LCD module
 */
uint8_t hw_read_status (void);
/**
 * Write a command byte to the LCD module
 * @param[in] aByte The command byte to be written to the LCD module
 */
void hw_write_cmd (uint8_t aCmd);
/**
 * Write a data byte to the LCD module
 * @param[in] aByte The data byte to be written to the LCD module
 */
void hw_write_data (uint8_t aData);
void hw_set_bit (THwIFaceExtraFlags aBit);
void hw_reset_bit (THwIFaceExtraFlags aBit);

#endif /* __AVR_LCD_PROJECT_HW_IFACE_H__ */
