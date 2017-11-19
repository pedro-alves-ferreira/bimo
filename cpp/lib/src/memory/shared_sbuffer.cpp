#include "bisect/bimo/memory/shared_buffer.h"
#include "bisect/bimo/idioms/enforce.h"
#include "gsl/span"

using namespace bisect::bimo;
using namespace bisect;

//------------------------------------------------------------------------------
byte_span bimo::as_span(sbuffer& b)
{
    return gsl::make_span(b.begin(), b.size());
}

cbyte_span bimo::as_span(const sbuffer& b)
{
    return gsl::make_span(b.cbegin(), b.size());
}
//------------------------------------------------------------------------------

sbuffer_ptr::sbuffer_ptr(sbuffer* buffer) : buffer_(buffer)
{
}

sbuffer_ptr::sbuffer_ptr(sbuffer_ptr& other)
{
    if (&other == this) return;

    this->buffer_ = other.buffer_;
    if (this->buffer_)
    {
        this->buffer_->add_ref();
    }
}

sbuffer_ptr::sbuffer_ptr(sbuffer_ptr&& other)
{ 
    BIMO_ASSERT(&other != this);
    this->buffer_ = other.buffer_;
    other.buffer_ = nullptr;
}

sbuffer_ptr& sbuffer_ptr::operator=(sbuffer_ptr& other)
{ 
    if (&other == this) return *this;

    this->buffer_ = other.buffer_;
    if (buffer_)
    {
        buffer_->add_ref();
    }

    return *this;
}

sbuffer_ptr& sbuffer_ptr::operator=(sbuffer_ptr&& other)
{ 
    if (&other == this) return *this;

    this->buffer_ = other.buffer_;
    other.buffer_ = nullptr;

    return *this;
}

sbuffer_ptr::~sbuffer_ptr()
{
    if (buffer_ == nullptr) return;
    buffer_->remove_ref();
}

sbuffer* sbuffer_ptr::get()
{ 
    return buffer_;
}

const sbuffer* sbuffer_ptr::get() const
{
    return buffer_;
}

sbuffer_ptr::operator bool() const
{
    return buffer_ != nullptr;
}

sbuffer* sbuffer_ptr::operator->()
{ 
    return buffer_;
}

const sbuffer* sbuffer_ptr::operator->() const
{ 
    return buffer_;
}

sbuffer& sbuffer_ptr::operator*()
{ 
    return *buffer_;
}

const sbuffer& sbuffer_ptr::operator*() const
{ 
    return *buffer_;
}
