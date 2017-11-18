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
        virtual byte* begin() = 0;
        virtual byte* end() = 0;
        virtual const byte* cbegin() const = 0;
        virtual const byte* cend() const = 0;
        virtual ptrdiff_t size() const = 0;

        virtual void add_ref() = 0;
        virtual void remove_ref() = 0;
    };

    using sbuffer = shared_buffer;

    class sbuffer_ptr
    {
    public:
        sbuffer_ptr(sbuffer* buffer = nullptr);
        sbuffer_ptr(sbuffer_ptr& other);
        sbuffer_ptr(sbuffer_ptr&& other);
        sbuffer_ptr& operator=(sbuffer_ptr& other);
        sbuffer_ptr& operator=(sbuffer_ptr&& other);
        ~sbuffer_ptr();

        sbuffer* get();
        const sbuffer* get() const;

        operator bool() const;
        sbuffer* operator->();
        const sbuffer* operator->() const;
        sbuffer& operator*();
        const sbuffer& operator*() const;

    private:
        sbuffer* buffer_ = nullptr;
    };

    template<class T>
    sbuffer_ptr make_sbuffer(size_t size)
    {
        return new T(size);
    }

    byte_span as_span(sbuffer& b);
    cbyte_span as_span(const sbuffer& b);
}
