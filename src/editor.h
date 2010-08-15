#ifndef __AVR_LCD_PROJECT_EDITOR_H__
#define __AVR_LCD_PROJECT_EDITOR_H__

#include "hw-rtc.h"

typedef void (*editor_done) (mbool aConfirmed, TRtcTimeInfo *aInfo);

typedef enum {
    EEditorFlagsNone    = 0x00U,
    EEditorFlagsSecond  = 0x01U,
    EEditorFlagsMinute  = 0x02U,
    EEditorFlagsHour    = 0x04U,
    EEditorFlagsDay     = 0x08U,
    EEditorFlagsMonth   = 0x10U,
    EEditorFlagsYear    = 0x20U,
} TEditorFlags;

void editor_init (void);
void editor_deinit (void);

void editor_activate (TEditorFlags aFlags, editor_done aCallback, const TRtcTimeInfo *aInitialInfo);
void editor_deactivate (void);

#endif /* __AVR_LCD_PROJECT_EDITOR_H__ */
