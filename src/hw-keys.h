#ifndef __AVR_LCD_PROJECT_HW_KEYS_H__
#define __AVR_LCD_PROJECT_HW_KEYS_H__

#include "types.h"

typedef enum {
    EHwKeyCodeNone,
    EHwKeyCodeKey1,
    EHwKeyCodeKey2,
    EHwKeyCodeKey3,
    EHwKeyCodeKey4,
    EHwKeyCodeKey5,
    EHwKeyCodeKeyUp,
    EHwKeyCodeKeyDown,
    EHwKeyCodeKeyLeft,
    EHwKeyCodeKeyRight,
    EHwKeyCodeKeyUpLeft,
    EHwKeyCodeKeyUpRight,
    EHwKeyCodeKeyDownLeft,
    EHwKeyCodeKeyDownRight,
    EHwKeyCodeKeyCenter
} THwKeyCode;

/**
 * Callback for the key event
 * @param[in] aCode The code defined by 'THwKeyCode'
 */
typedef void (*hw_keys_on_key_event) (muint8 aCode);

/**
 * Initialize the keyboard system
 */
void hw_keys_init (void);
/**
 * Deinitialize the keyboard system
 */
void hw_keys_deinit (void);
/**
 * Add a callback for a key event
 */
void hw_keys_add_key_event_handler (hw_keys_on_key_event aCallback);
/**
 * Remove a callback for a key event
 */
void hw_keys_remove_key_event_handler (hw_keys_on_key_event aCallback);

#endif /* __AVR_LCD_PROJECT_HW_KEYS_H__ */
