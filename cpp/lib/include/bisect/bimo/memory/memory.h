#pragma once

#include "bisect/bimo/types.h"

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    void copy(cbyte_span from, byte_span to);

    template <class C1, class C2> bool equal(const C1& c1, const C2& c2)
    {
        if(ptrdiff_t(size(c1)) != ptrdiff_t(size(c2))) return false;

        const auto bg2     = begin(c2);
        const auto [m1, _] = std::mismatch(begin(c1), end(c1), bg2, end(c2));
        (void)_;
        return (m1 == end(c1));
    }
} // namespace bisect::bimo
