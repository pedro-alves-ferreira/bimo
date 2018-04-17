#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/memory/telemetry.h"
#include "bisect/bimo/idioms/enforce.h"

using namespace bisect::bimo;

//------------------------------------------------------------------------------

malloc_sbuffer::malloc_sbuffer(size_t _size,
    buffer_recycler_wptr recycler)
    : recycler_(recycler),
    size_(_size)
{
    base_ = _size == 0 ? nullptr : reinterpret_cast<byte*>(::malloc(size_));
    telemetry::instance().on_direct_allocation(_size);
}

malloc_sbuffer::~malloc_sbuffer()
{
    ::free(base_);
}

byte* malloc_sbuffer::begin() noexcept
{
    return base_;
}

byte* malloc_sbuffer::end() noexcept
{
    return base_ + size_;
}

const byte* malloc_sbuffer::cbegin() const noexcept
{
    return base_;
}

const byte* malloc_sbuffer::cend() const noexcept
{
    return base_ + size_;
}

ptrdiff_t malloc_sbuffer::size() const noexcept
{
    return size_;
}

void malloc_sbuffer::add_ref() noexcept
{
    ++ref_count_;
}

void malloc_sbuffer::remove_ref() noexcept
{
    BIMO_ASSERT(ref_count_ > 0);

    if (--ref_count_ == 0)
    {
        ref_count_ = 1;

        auto r = recycler_.lock();

        if (r)
        {
            r->recycle(this);
        }
        else
        {
            delete this;
        }
    }
}
