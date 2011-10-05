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

static const muint16 *TheNotePointer = 0;
static muint16 TheNones1[] M_FLASH = {
    0x0440U, 0x0449U, 0x0449U, 0x0447U,
    0x0449U, 0x0445U, 0x0440U, 0x0440U,
    0x0440U, 0x0449U, 0x0449U, 0x0447U,
    0x0452U, 0x0850U, 0x0450U, 0x0442U,
    0x0442U, 0x044AU, 0x044AU, 0x0449U,
    0x0447U, 0x0445U, 0x0440U, 0x0449U,
    0x0449U, 0x0447U, 0x0449U, 0x0845U,
    0xFFFFU
};

static muint16 TheNones2[] M_FLASH = {
    0x0450U, 0x0452U, 0x0454U, 0x0457U,
    0x0456U, 0x0855U, 0x044BU, 0x0450U,
    0x0452U, 0x0454U, 0x0455U, 0x0655U,
    0x0854U, 0x0449U, 0x044BU, 0x0450U,
    0x0454U, 0x0453U, 0x0852U, 0x0450U,
    0x0449U, 0x064BU, 0x0450U, 0x0449U,
    0x084BU, 0x0450U, 0x0452U, 0x0454U,
    0x0457U, 0x0456U, 0x0855U, 0x044BU,
    0x0450U, 0x0452U, 0x0454U, 0x0455U, 0x0655U,
    0x0654U, 0x0654U, 0x0652U, 0x0449U,
    0x0652U, 0x0649U, 0x0650U, 0x0449U,
    0x0650U, 0x0449U, 0x044BU, 0x0453U,
    0x0456U, 0x0459U, 0x0658U,
    0xFFFFU
};

static void alarm_sound_finished (void) {
    if (TheNotePointer) {
        const muint16 note = pgm_read_word (TheNotePointer);
        if (0xFFFFU == note) {
            /* the last note has been played, stop playing */
            TheAlarmState = EAlarmStateIdle;
            TheNotePointer = NULL;
            if (TheCallback) {
                (*TheCallback)(TRUE);
                TheCallback = NULL;
            }
        }
        else {
            const muint8 length = (note>>8)&0x0FFU;

            hw_sound_play_note ((note & 0xFFU), length*125, &alarm_sound_finished);
            ++TheNotePointer;
        }
    }
}

void alarm_play_melody (TAlarmMelody aMelody, alarm_play_melody_done aCallback) {
    /* check the alarm state */
    if (EAlarmStatePlaying == TheAlarmState) {
        if (aCallback) {
            (*aCallback)(FALSE);
        }
        return;
    }
    if (EAlarmMelodyNone == aMelody) {
        if (aCallback) {
            (*aCallback)(TRUE);
        }
        return;
    }
    else {
        if (EAlarmMelodyCTree == aMelody) {
            TheNotePointer = TheNones1;
        }
        else /* if (EAlarmMelodyHare == aMelody) */ {
            TheNotePointer = TheNones2;
        }

        TheAlarmState = EAlarmStatePlaying;
        TheCallback = aCallback;
        alarm_sound_finished ();
    }
}
