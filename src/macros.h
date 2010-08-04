#ifndef __
#define __

#include <stdio.h>

#define m_return_if_fail(arg) \
    if (!(arg)) { \
        fprintf (stderr, "Warning: function: %s; failed: [%s]\n", __func__, #arg); \
        return; \
    }
#define m_return_val_if_fail(arg,val) \
    if (!(arg)) { \
        fprintf (stderr, "Warning: function: %s; failed: [%s]\n", __func__, #arg); \
        return val; \
    }

#define M_FLASH

#endif /**/
