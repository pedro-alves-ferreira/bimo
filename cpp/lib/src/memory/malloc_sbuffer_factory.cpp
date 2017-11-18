#include "bisect/bimo/memory/malloc_sbuffer_factory.h"
#include "bisect/bimo/memory/malloc_sbuffer.h"

using namespace bisect::bimo;

//------------------------------------------------------------------------------

sbuffer_ptr malloc_sbuffer_factory::get_buffer(size_t buffer_size)
{
    return sbuffer_ptr(new malloc_sbuffer(buffer_size));
}
