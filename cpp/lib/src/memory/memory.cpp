#include "bisect/bimo/memory/memory.h"
#include "bisect/bimo/idioms/enforce.h"

//------------------------------------------------------------------------------

void bisect::bimo::copy(cbyte_span from, byte_span to)
{
    BIMO_ASSERT(from.size() <= to.size());

#if _MSC_VER
    {
        const auto copy_result = memcpy_s(to.data(), to.size(), from.data(), from.size());
        BIMO_ASSERT(copy_result == 0);
    }
#else
    {
        memcpy(to.data(), from.data(), from.size());
    }
#endif
}
