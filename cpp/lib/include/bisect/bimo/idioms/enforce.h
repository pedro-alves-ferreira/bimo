#pragma once

#include <iostream>
#include <cassert>

//------------------------------------------------------------------------------

namespace bisect::bimo::detail
{
    template<typename ExceptionType, typename MsgType>
    inline void do_throw(MsgType error)
    {
        throw ExceptionType(error);
    }
}

#define BIMO_ENFORCE(_condition, _exception_type, error_msg) \
    if(!(_condition)) \
    { \
        bisect::bimo::detail::do_throw<_exception_type>(error_msg); \
    }

//------------------------------------------------------------------------------

#if defined(NDEBUG)
#   define BIMO_ASSERT(_condition)
#else // defined(NDEBUG)
#   define BIMO_ASSERT(_condition) assert(_condition)
#endif // defined(NDEBUG)
