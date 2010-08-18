#ifndef __AVR_LCD_PROJECT_HW_SOUND_H__
#define __AVR_LCD_PROJECT_HW_SOUND_H__

#include "types.h"

/**
 * The callback function to notify that the note is finished
 */
typedef void (*hw_sound_finished) (void);

/**
 * Initialize the sound hardware
 */
void hw_sound_init (void);
/**
 * Deinitialize the sound hardware
 */
void hw_sound_deinit (void);

/**
 * Play a note:
 * --------------------    ----------------------
 * |  # | Note | Code |    |  # | Octave | Code |
 * --------------------    ----------------------
 * |  1 |   C  | 0x00 |    |  1 |     0  | ---- |
 * |  2 |   C# | 0x01 |    |  2 |     1  | ---- |
 * |  3 |   D  | 0x02 |    |  3 |     2  | ---- |
 * |  4 |   D# | 0x03 |    |  4 |     3  | ---- |
 * |  5 |   E  | 0x04 |    |  5 |    (4) | 0x40 |
 * |  6 |   F  | 0x05 |    |  6 |     5  | 0x50 |
 * |  7 |   F# | 0x06 |    |  7 |     6  | 0x60 |
 * |  8 |   G  | 0x07 |    |  8 |     7  | 0x70 |
 * |  9 |   G# | 0x08 |    |  9 |     8  | 0x80 |
 * | 10 |  (A) | 0x09 |    ----------------------
 * | 11 |   B  | 0x0A |
 * | 12 |   H  | 0x0B |
 * --------------------
 * @param[in] aNote The Note and Octave information
 * @param[in] aLength The length of the note in msec
 * @param[in] aCallback The callback when the note is finished
 */
void hw_sound_play_note (muint8 aNote, muint16 aLength, hw_sound_finished aCallback);

#endif /* __AVR_LCD_PROJECT_HW_SOUND_H__ */
