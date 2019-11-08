#pragma once

#include "bisect/bimo/types.h"
#include <memory>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    // Based on ACE
    class shared_buffer
    {
      protected:
        virtual ~shared_buffer() = default;

      public:
        virtual byte* begin() noexcept              = 0;
        virtual byte* end() noexcept                = 0;
        virtual const byte* cbegin() const noexcept = 0;
        virtual const byte* cend() const noexcept   = 0;
        virtual ptrdiff_t size() const noexcept     = 0;

        virtual void add_ref() noexcept    = 0;
        virtual void remove_ref() noexcept = 0;
    };

    using sbuffer = shared_buffer;

    class sbuffer_ptr
    {
      public:
        sbuffer_ptr(sbuffer* buffer = nullptr) noexcept;
        sbuffer_ptr(sbuffer_ptr& other) noexcept;
        sbuffer_ptr(sbuffer_ptr&& other) noexcept;
        sbuffer_ptr& operator=(sbuffer_ptr& other) noexcept;
        sbuffer_ptr& operator=(sbuffer_ptr&& other) noexcept;
        ~sbuffer_ptr();

        sbuffer* get() noexcept;
        const sbuffer* get() const noexcept;

        explicit operator bool() const noexcept;
        sbuffer* operator->() noexcept;
        const sbuffer* operator->() const noexcept;
        sbuffer& operator*() noexcept;
        const sbuffer& operator*() const noexcept;

      private:
        sbuffer* buffer_ = nullptr;
    };

    byte_span as_span(sbuffer& b) noexcept;
    cbyte_span as_span(const sbuffer& b) noexcept;
} // namespace bisect::bimo
