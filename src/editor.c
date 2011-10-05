#include "editor.h"

#include "macros.h"
#include "hw-keys.h"
#include "lcd-driver.h"

#include <string.h>

typedef enum {
    EEditorStateNull = 0,
    EEditorStateIdle,
    EEditorStateActive
} TEditorState;

static muint8 TheIndex = 0;
static muint8 TheEditorState = EEditorStateNull;

static editor_done TheCallback = NULL;
static TRtcTimeInfo TheTimeInfo;

static void editor_on_key_event (muint8 aCode);
static void editor_update_screen (void);

void editor_init (void) {
    TheEditorState = EEditorStateIdle;

    hw_keys_add_key_event_handler (&editor_on_key_event);
}

#ifndef M_NO_DEINIT
void editor_deinit (void) {
    hw_keys_remove_key_event_handler (&editor_on_key_event);
}
#endif /* !M_NO_DEINIT */

void editor_activate (TEditorFlags aFlags,
    editor_done aCallback, const TRtcTimeInfo *aInitialInfo) {
    M_UNUSED_PARAM (aFlags);

    m_return_if_fail (aCallback);
    m_return_if_fail (EEditorStateIdle == TheEditorState);

    TheCallback = aCallback;

    if (aInitialInfo) {
        memcpy (&TheTimeInfo, aInitialInfo, sizeof (TRtcTimeInfo));
    }
    else {
        memset (&TheTimeInfo, 0, sizeof (TRtcTimeInfo));
    }

    TheIndex = 0;
    editor_update_screen ();

    TheEditorState = EEditorStateActive;
}

static void editor_update_screen (void) {
    lcd_clear ();
    lcd_paint_char_7x14 (0, 1, '0' + ((TheTimeInfo.mHour>>4)&0x0FU));
    lcd_paint_char_7x14 (7, 1, '0' + ((TheTimeInfo.mHour)&0x0FU));
    lcd_paint_char_7x14 (14, 1, ':');
    lcd_paint_char_7x14 (21, 1, '0' + ((TheTimeInfo.mMinute>>4)&0x0FU));
    lcd_paint_char_7x14 (28, 1, '0' + ((TheTimeInfo.mMinute)&0x0FU));
    lcd_paint_char_7x14 (40, 1, 'O');
    lcd_paint_char_7x14 (47, 1, 'K');
    lcd_paint_char_7x14 (
        (0 == TheIndex) ? 0 :
        (1 == TheIndex) ? 7 :
        (2 == TheIndex) ? 21 :
        (3 == TheIndex) ? 28 : 40, 3, '_');
    if (4 == TheIndex) {
        lcd_paint_char_7x14 (47, 3, '_');
    }
    lcd_flash ();
}

void editor_deactivate (void) {
    TheCallback = NULL;
    TheEditorState = EEditorStateIdle;
}

static void editor_inc_hour (mbool aHighPart) {
    if (aHighPart) {
        TheTimeInfo.mHour = TheTimeInfo.mHour + 0x10U;
        if ((TheTimeInfo.mHour&0xF0U) == 0x30U) {
            TheTimeInfo.mHour = TheTimeInfo.mHour&0x0FU;
        }
        else if (((TheTimeInfo.mHour&0xF0U) == 0x20U) &&
            ((TheTimeInfo.mHour &0x0F) > 3)) {
            TheTimeInfo.mHour = (TheTimeInfo.mHour&0xF0) | 0x03U;
        }
    }
    else {
        TheTimeInfo.mHour = TheTimeInfo.mHour + 0x01U;
        if ((TheTimeInfo.mHour&0xF0U) == 0x20U) {
            if ((TheTimeInfo.mHour&0x0FU) > 3) {
                TheTimeInfo.mHour = (TheTimeInfo.mHour&0xF0);
            }
        }
        else {
            if ((TheTimeInfo.mHour&0x0FU) > 9) {
                TheTimeInfo.mHour = (TheTimeInfo.mHour&0xF0);
            }
        }
    }
}

static void editor_dec_hour (mbool aHighPart) {
    if (aHighPart) {
        if (TheTimeInfo.mHour&0xF0U) {
            TheTimeInfo.mHour = TheTimeInfo.mHour - 0x10U;
        }
        else {
            TheTimeInfo.mHour = (TheTimeInfo.mHour & 0x0FU) | 0x20U;
            if ((TheTimeInfo.mHour&0x0FU) > 0x03) {
                TheTimeInfo.mHour = (TheTimeInfo.mHour&0xF0) | 0x03U;
            }
        }
    }
    else {
        if (TheTimeInfo.mHour&0x0FU) {
            --TheTimeInfo.mHour;
        }
        else {
            TheTimeInfo.mHour = ((TheTimeInfo.mHour & 0xF0) == 0x20) ?
                ((TheTimeInfo.mHour&0xF0U) | 0x03U): ((TheTimeInfo.mHour&0xF0U) | 0x09U);
        }
    }
}

static void editor_inc_minute (mbool aHighPart) {
    if (aHighPart) {
        TheTimeInfo.mMinute = TheTimeInfo.mMinute + 0x10U;
        if ((TheTimeInfo.mMinute&0xF0U) == 0x60U) {
            TheTimeInfo.mMinute = TheTimeInfo.mMinute&0x0FU;
        }
    }
    else {
        TheTimeInfo.mMinute = TheTimeInfo.mMinute + 0x01U;
        if ((TheTimeInfo.mMinute&0x0FU) > 9) {
            TheTimeInfo.mMinute = (TheTimeInfo.mMinute&0xF0);
        }
    }
}

static void editor_dec_minute (mbool aHighPart) {
    if (aHighPart) {
        if (TheTimeInfo.mMinute&0xF0U) {
            TheTimeInfo.mMinute = TheTimeInfo.mMinute - 0x10U;
        }
        else {
            TheTimeInfo.mMinute = (TheTimeInfo.mMinute & 0x0FU) | 0x50U;
        }
    }
    else {
        if (TheTimeInfo.mMinute&0x0FU) {
            --TheTimeInfo.mMinute;
        }
        else {
            TheTimeInfo.mMinute = (TheTimeInfo.mMinute&0xF0U) | 0x09U;
        }
    }
}

static void editor_on_key_event (muint8 aCode) {
    if (EEditorStateActive == TheEditorState) {
        if (EHwKeyCodeKey1 == aCode) {
            if (EHwKeyCodeKey1 == aCode) {
                // handle 'done'
                if (TheCallback) {
                    (*TheCallback)(TRUE, &TheTimeInfo);
                }
                TheEditorState = EEditorStateIdle;
            }
        }
        else if (EHwKeyCodeKey2 == aCode) {
            TheIndex -= 1;
            if (TheIndex == 255) {
                TheIndex = 4;
            }
        }
        else if (EHwKeyCodeKey3 == aCode) {
            TheIndex += 1;
            if (TheIndex == 5) {
                TheIndex = 0;
            }
        }
        else if (EHwKeyCodeKey4 == aCode) {
            if (0 == TheIndex) {
                editor_dec_hour (TRUE);
            }
            else if (1 == TheIndex) {
                editor_dec_hour (FALSE);
            }
            else if (2 == TheIndex) {
                editor_dec_minute (TRUE);
            }
            else if (3 == TheIndex) {
                editor_dec_minute (FALSE);
            }
        }
        else if (EHwKeyCodeKey5 == aCode) {
            if (0 == TheIndex) {
                editor_inc_hour (TRUE);
            }
            else if (1 == TheIndex) {
                editor_inc_hour (FALSE);
            }
            else if (2 == TheIndex) {
                editor_inc_minute (TRUE);
            }
            else if (3 == TheIndex) {
                editor_inc_minute (FALSE);
            }
        }

        editor_update_screen ();
    }
}
