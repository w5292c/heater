#include "hw-i2c.h"

#include "macros.h"
#include "scheduler.h"

/**
 * The current 'read' request callback
 */
static hw_i2c_read_done TheReadCallback;
/**
 * The current 'write' request callback
 */
static hw_i2c_write_done TheWriteCallback;

/**
 * The scheduler tick
 * @return TRUE if more work is already available
 */
static mbool hw_i2c_sched_tick (void);

void hw_i2c_init (void) {
    TheReadCallback = NULL;
    TheWriteCallback = NULL;

    scheduler_add (&hw_i2c_sched_tick);
}

void hw_i2c_deinit (void) {
    scheduler_remove (&hw_i2c_sched_tick);

    TheReadCallback = NULL;
    TheWriteCallback = NULL;
}

void hw_i2c_read (muint8 aAddress, muint8 *aBuffer, msize aSize, hw_i2c_read_done aCallback) {
    m_return_if_fail (aCallback);

    M_UNUSED_PARAM (aSize);
    M_UNUSED_PARAM (aBuffer);
    M_UNUSED_PARAM (aAddress);

    if (!TheReadCallback && !TheWriteCallback) {
        TheReadCallback = aCallback;
    }
    else {
        (*aCallback) (FALSE, 0);
    }
}

void hw_i2c_write (muint8 aAddress, const muint8 *aBuffer,
    msize aSize, hw_i2c_write_done aCallback) {
    m_return_if_fail (aCallback);

    M_UNUSED_PARAM (aSize);
    M_UNUSED_PARAM (aBuffer);
    M_UNUSED_PARAM (aAddress);

    if (!TheWriteCallback && !TheReadCallback) {
        TheWriteCallback = aCallback;
    }
    else {
        (*aCallback) (FALSE, 0);
    }
}

static mbool hw_i2c_sched_tick (void) {
    return FALSE;
}
