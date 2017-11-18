#pragma once

#include "bisect/bimo/memory/shared_buffer.h"
#include <memory>
#include <atomic>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class malloc_sbuffer : public shared_buffer
    {
    public:
        explicit malloc_sbuffer(size_t _size);

        byte* begin() override;
        byte* end() override;
        const byte* cbegin() const override;
        const byte* cend() const override;
        ptrdiff_t size() const override;

        void add_ref() override;
        void remove_ref() override;

    private:
        ~malloc_sbuffer();

    private:
        std::atomic<int> ref_count_ = 1;
        size_t size_ = 0;
        byte* base_ = nullptr;
    };
}
