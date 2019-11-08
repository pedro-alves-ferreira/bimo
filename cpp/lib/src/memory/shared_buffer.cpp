#include "bisect/bimo/memory/shared_buffer.h"
#include "bisect/bimo/idioms/enforce.h"
#include "gsl/span"

using namespace bisect::bimo;
using namespace bisect;

//------------------------------------------------------------------------------
byte_span bimo::as_span(sbuffer& b) noexcept
{
    return gsl::make_span(b.begin(), b.size());
}

cbyte_span bimo::as_span(const sbuffer& b) noexcept
{
    return gsl::make_span(b.cbegin(), b.size());
}
//------------------------------------------------------------------------------

sbuffer_ptr::sbuffer_ptr(sbuffer* buffer) noexcept : buffer_(buffer)
{
}

sbuffer_ptr::sbuffer_ptr(sbuffer_ptr& other) noexcept : buffer_(other.buffer_)
{
    if(&other == this) return;

    if(this->buffer_)
    {
        this->buffer_->add_ref();
    }
}

sbuffer_ptr::sbuffer_ptr(sbuffer_ptr&& other) noexcept : buffer_(other.buffer_)
{
    BIMO_ASSERT(&other != this);
    other.buffer_ = nullptr;
}

sbuffer_ptr& sbuffer_ptr::operator=(sbuffer_ptr& other) noexcept
{
    if(&other == this) return *this;

    if(buffer_)
    {
        buffer_->remove_ref();
    }

    this->buffer_ = other.buffer_;

    if(buffer_)
    {
        buffer_->add_ref();
    }

    return *this;
}

sbuffer_ptr& sbuffer_ptr::operator=(sbuffer_ptr&& other) noexcept
{
    if(&other == this) return *this;

    if(buffer_)
    {
        buffer_->remove_ref();
    }

    this->buffer_ = other.buffer_;
    other.buffer_ = nullptr;

    return *this;
}

sbuffer_ptr::~sbuffer_ptr()
{
    if(buffer_ == nullptr) return;
    buffer_->remove_ref();
}

sbuffer* sbuffer_ptr::get() noexcept
{
    return buffer_;
}

const sbuffer* sbuffer_ptr::get() const noexcept
{
    return buffer_;
}

sbuffer_ptr::operator bool() const noexcept
{
    return buffer_ != nullptr;
}

sbuffer* sbuffer_ptr::operator->() noexcept
{
    return buffer_;
}

const sbuffer* sbuffer_ptr::operator->() const noexcept
{
    return buffer_;
}

sbuffer& sbuffer_ptr::operator*() noexcept
{
    return *buffer_;
}

const sbuffer& sbuffer_ptr::operator*() const noexcept
{
    return *buffer_;
}
