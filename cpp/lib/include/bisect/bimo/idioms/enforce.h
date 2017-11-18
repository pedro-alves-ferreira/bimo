#pragma once

#include <iostream>
#include <cassert>

//------------------------------------------------------------------------------

namespace bisect::bimo::detail
{
    template<typename ExceptionType>
    inline void do_throw(const char _condition[])
    {
        throw ExceptionType(_condition);
    }
}

#define BIMO_ENFORCE(_condition, _exception_type, ...) \
    if(!(_condition)) \
    { \
        bisect::bimo::detail::do_throw<_exception_type>(#_condition); \
    }

#define BIMO_THROW(_exception_type, ...) \
    bisect::bimo::detail::do_throw<_exception_type>("exception"); \

//------------------------------------------------------------------------------


#if defined(NDEBUG)
#   define BIMO_ASSERT(_condition)
#else // defined(NDEBUG)
#   define BIMO_ASSERT(_condition) assert(_condition)
#endif // defined(NDEBUG)
