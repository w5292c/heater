#ifndef __M_AVR_PROJECT_ALARM_H__
#define __M_AVR_PROJECT_ALARM_H__

#include "types.h"

typedef void (*alarm_play_melody_done)(mbool aSuccess);

typedef enum {
    EAlarmMelodyNone,
    EAlarmMelodyCTree
} TAlarmMelody;

/**
 * Play the melody defined with 'aMelody'
 * @param[in] aMelody The melody ID to be played
 * @param[in] aCallback The callback to be invoked when the melody is finished
 */
void alarm_play_melody (TAlarmMelody aMelody, alarm_play_melody_done aCallback);

#endif /* __M_AVR_PROJECT_ALARM_H__ */
