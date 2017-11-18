#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/idioms/enforce.h"

using namespace bisect::bimo;

//------------------------------------------------------------------------------

malloc_sbuffer::malloc_sbuffer(size_t _size)
    : size_(_size)
{
    base_ = _size == 0 ? nullptr : reinterpret_cast<byte*>(::malloc(size_));
}

malloc_sbuffer::~malloc_sbuffer()
{
    ::free(base_);
}

byte* malloc_sbuffer::begin()
{
    return base_;
}

byte* malloc_sbuffer::end()
{
    return base_ + size_;
}

const byte* malloc_sbuffer::cbegin() const
{
    return base_;
}

const byte* malloc_sbuffer::cend() const
{
    return base_ + size_;
}

ptrdiff_t malloc_sbuffer::size() const
{
    return size_;
}

void malloc_sbuffer::add_ref()
{
    ++ref_count_;
}

void malloc_sbuffer::remove_ref()
{
    BIMO_ASSERT(ref_count_ > 0);

    if (--ref_count_ == 0)
    {
        delete this;
    }
}
