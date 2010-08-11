#include "hw-i2c.h"

#include "macros.h"
#include "scheduler.h"

static mbool hw_i2c_sched_tick (void);

#if 0
#define M_HW_I2C_MAX_REQUESTS (0x02U)

typedef void (*hw_i2c_read_done) (mbool aSuccess, msize aBytesRead);
typedef void (*hw_i2c_write_done) (mbool aSuccess, msize aBytesWritten);
#endif

void hw_i2c_init (void) {
    scheduler_add (&hw_i2c_sched_tick);
}

void hw_i2c_deinit (void) {
    scheduler_remove (&hw_i2c_sched_tick);
}

void hw_i2c_read (muint8 aAddress, muint8 *aBuffer, msize aSize, hw_i2c_read_done aCallback) {
    M_UNUSED_PARAM (aSize);
    M_UNUSED_PARAM (aBuffer);
    M_UNUSED_PARAM (aAddress);
    M_UNUSED_PARAM (aCallback);
}

void hw_i2c_write (muint8 aAddress, const muint8 *aBuffer,
    msize aSize, hw_i2c_write_done aCallback) {
    M_UNUSED_PARAM (aSize);
    M_UNUSED_PARAM (aBuffer);
    M_UNUSED_PARAM (aAddress);
    M_UNUSED_PARAM (aCallback);
}

static mbool hw_i2c_sched_tick (void) {
    return FALSE;
}
