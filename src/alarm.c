#include "alarm.h"

#ifdef M_AVR
#include <avr/pgmspace.h>
#endif
#include "hw-sound.h"
#include "macros.h"

typedef enum {
    EAlarmStateIdle,
    EAlarmStatePlaying
} TAlarmState;

static void alarm_sound_finished (void);

static alarm_play_melody_done TheCallback = NULL;
static TAlarmState TheAlarmState = EAlarmStateIdle;

static muint8 TheNoteIndex = 0;
static muint16 TheNones[] M_FLASH = {
    0x0440U, 0x0449U, 0x0449U, 0x0447U,
    0x0449U, 0x0445U, 0x0440U, 0x0440U,
    0x0440U, 0x0449U, 0x0449U, 0x0447U,
    0x0452U, 0x0850U, 0x0450U, 0x0442U,
    0x0442U, 0x044AU, 0x044AU, 0x0449U,
    0x0447U, 0x0445U, 0x0440U, 0x0449U,
    0x0449U, 0x0447U, 0x0449U, 0x0845U,
    0xFFFFU
};

static void alarm_sound_finished (void) {
    const muint16 note = pgm_read_word (&TheNones[TheNoteIndex]);
    if (0xFFFFU == note) {
        /* the last note has been played, stop playing */
        TheAlarmState = EAlarmStateIdle;
        if (TheCallback) {
            (*TheCallback)(TRUE);
            TheCallback = NULL;
        }
        return;
    }
    const muint8 length = (note>>8)&0x0FFU;

    hw_sound_play_note ((note & 0xFFU), length*125, &alarm_sound_finished);
    ++TheNoteIndex;
}

void alarm_play_melody (TAlarmMelody aMelody, alarm_play_melody_done aCallback) {
    if (EAlarmMelodyNone == aMelody) {
        if (aCallback) {
            (*aCallback)(TRUE);
        }
        return;
    }
    /* check the alarm state */
    if (EAlarmStatePlaying == TheAlarmState) {
        if (aCallback) {
            (*aCallback)(FALSE);
        }
        return;
    }

    TheAlarmState = EAlarmStatePlaying;
    TheNoteIndex = 0;
    TheCallback = aCallback;
    alarm_sound_finished ();
}
