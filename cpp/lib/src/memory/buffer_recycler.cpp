#include "bisect/bimo/memory/buffer_recycler.h"
#include "bisect/bimo/memory/malloc_sbuffer.h"
using namespace bisect::bimo;

//------------------------------------------------------------------------------

void buffer_recycler::delete_sbuffer(malloc_sbuffer* b)
{
    delete b;
}
