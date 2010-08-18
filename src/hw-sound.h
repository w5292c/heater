#ifndef __AVR_LCD_PROJECT_HW_SOUND_H__
#define __AVR_LCD_PROJECT_HW_SOUND_H__

#include "types.h"

typedef void (*hw_sound_finished) (void);

void hw_sound_init (void);
void hw_sound_deinit (void);

void hw_sound_play_note (muint8 aNote, muint16 aLength, hw_sound_finished aCallback);

#endif /* __AVR_LCD_PROJECT_HW_SOUND_H__ */
