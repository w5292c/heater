#include "hw-sound.h"

#include "macros.h"
#include "hw-timer.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#endif

typedef enum {
    EHwSoundStateNull = 0,
    EHwSoundStateIdle,
    EHwSoundStatePlaying,
} THwSoundState;

static muint8 TheSoundState = EHwSoundStateNull;
static muint16 TheNoteLength = 0;
static hw_sound_finished TheCallback = NULL;

static void hw_sound_timer_callback (void);

static mint16 TheBase[] M_FLASH = {
     61155U, /*< C  : 000.00 Hz */
     57724U, /*< C# : 000.00 Hz */
     54485U, /*< D  : 000.00 Hz */
     51425U, /*< D# : 000.00 Hz */
     48539U, /*< E  : 000.00 Hz */
     45815U, /*< F  : 000.00 Hz */
     43244U, /*< F# : 000.00 Hz */
     40816U, /*< G  : 000.00 Hz */
     38526U, /*< G# : 000.00 Hz */
     36364U, /*< A  : 440.00 Hz */
     34323U, /*< B  : 000.00 Hz */
     32397U  /*< H  : 000.00 Hz */
};

static muint16 ThePreScalers[] M_FLASH = {
    1, 8, 64, 256, 1024
};

void hw_sound_init (void) {
    m_return_if_fail (EHwSoundStateNull == TheSoundState);

    hw_timer_add_callback (&hw_sound_timer_callback);
    TheSoundState = EHwSoundStateIdle;
}

void hw_sound_deinit (void) {
    hw_timer_remove_callback (&hw_sound_timer_callback);
    TheSoundState = EHwSoundStateNull;
}

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
 *
 * Base: A4 - 440 Hz
 * Fo (note frequency) = Fbase / (2*X*(1 + Y))
 * X - is the prescaler, possible values: 1, 8, 64, 256, 1024
 *       Fbase
 * Y = ---------- - 1
 *     Fo * 2 * X
 *
 *       Fbase
 * Noi= -------
 *        Foi
 *
 *      Noi
 * Y = ----- - 1
 *     2 * X
 */
void hw_sound_play_note (muint8 aNote, muint16 aLength, hw_sound_finished aCallback) {
    muint i;
    muint8 note;
    mint8 octave;
    muint16 base;
    mint16 count;
    muint16 prescaler;
    muint8 prescaler_ctrl;

    m_return_if_fail (aCallback);
    m_return_if_fail (EHwSoundStateIdle == TheSoundState || ((*aCallback) (), FALSE));

    if (0xFFU != aNote) {
        note = (aNote & 0x0FU);
        m_return_if_fail (note <= 0x0B || ((*aCallback) (), FALSE));
        octave = ((aNote>>4) & 0x0FU) - 4;
        m_return_if_fail ((octave >= 0 && octave < 5) || ((*aCallback) (), FALSE));

        base = pgm_read_word (&TheBase[note]);
        while (octave--) {
            base >>= 1;
        }
        for (i = 0; i < 5; i++) {
            prescaler = pgm_read_word (&ThePreScalers[i]);
            count = base/(prescaler*2) - 1;
            if (count > 0 && count < 0x100) {
                break;
            }
        }

        /* now, 'count' and 'prescaler' have correct values */
        m_return_if_fail ((count > 0 && count < 0x100) || ((*aCallback) (), FALSE));
        m_return_if_fail ((
            prescaler == 1 || prescaler == 8 ||
            prescaler == 64 || prescaler == 256 ||
            prescaler == 1024) || ((*aCallback) (), FALSE));

        switch (prescaler)
        {
        case 1:
            prescaler_ctrl = (0<<CS02)|(0<<CS01)|(1<<CS00);
            break;
        case 8:
            prescaler_ctrl = (0<<CS02)|(1<<CS01)|(0<<CS00);
            break;
        case 64:
            prescaler_ctrl = (0<<CS02)|(1<<CS01)|(1<<CS00);
            break;
        case 256:
            prescaler_ctrl = (1<<CS02)|(0<<CS01)|(0<<CS00);
            break;
        case 1024:
            prescaler_ctrl = (1<<CS02)|(0<<CS01)|(1<<CS00);
            break;
        default:
            /* incorrect prescaler value */
            m_return_if_fail ((FALSE && prescaler) || ((*aCallback) (), FALSE));
        break;
        }

        TCNT0 = 0;
        OCR0 = (muint8)count;
        TCCR0 =
            (1<<WGM01)|(0<<WGM00)| /*< Counter mode: CTC */
            (0<<COM01)|(1<<COM00)| /*< Output mode: Toggle output */
            prescaler_ctrl; /*< prescaler control value */
    }

    TheSoundState = EHwSoundStatePlaying;
    TheNoteLength = aLength;
    TheCallback = aCallback;
}

static void hw_sound_timer_callback (void) {
    if (EHwSoundStatePlaying == TheSoundState) {
        if (0 == --TheNoteLength) {
            TCCR0 = 0;
            TheSoundState = EHwSoundStateIdle;

            m_return_if_fail (TheCallback);
            (*TheCallback) ();
        }
    }
}
