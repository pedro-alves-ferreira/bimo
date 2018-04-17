#pragma once

#include "bisect/bimo/memory/shared_buffer.h"
#include "bisect/bimo/memory/buffer_recycler.h"
#include <memory>
#include <atomic>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class malloc_sbuffer : public shared_buffer
    {
    public:
        explicit malloc_sbuffer(size_t _size, buffer_recycler_wptr recycler);
        ~malloc_sbuffer();

        byte* begin() noexcept override;
        byte* end() noexcept override;
        const byte* cbegin() const noexcept override;
        const byte* cend() const noexcept override;
        ptrdiff_t size() const noexcept override;

        void add_ref() noexcept override;
        void remove_ref() noexcept override;

    private:
        buffer_recycler_wptr recycler_;
        std::atomic<int> ref_count_ = 1;
        size_t size_ = 0;
        byte* base_ = nullptr;
    };

    using malloc_sbuffer_uptr = std::unique_ptr<malloc_sbuffer>;
}
