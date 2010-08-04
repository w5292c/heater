#include "fonts.h"
#include "macros.h"

mubyte get_font14_byte (mubyte aChar, mubyte aLine) {
    m_return_val_if_fail (aChar > 31, 0);
    m_return_val_if_fail (aChar < 128, 0);
    m_return_val_if_fail (aLine < 14, 0);

    return 123;
}
