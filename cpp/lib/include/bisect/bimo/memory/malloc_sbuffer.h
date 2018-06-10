#pragma once

#include "bisect/bimo/memory/shared_buffer.h"
#include "bisect/bimo/memory/buffer_recycler.h"
#include <memory>
#include <atomic>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    void sbuffer_deleter(malloc_sbuffer*);

    class malloc_sbuffer : public shared_buffer
    {
    public:
        explicit malloc_sbuffer(size_t _size, buffer_recycler_wptr recycler);

        byte* begin() noexcept override;
        byte* end() noexcept override;
        const byte* cbegin() const noexcept override;
        const byte* cend() const noexcept override;
        ptrdiff_t size() const noexcept override;

        void add_ref() noexcept override;
        void remove_ref() noexcept override;

    private:
        malloc_sbuffer(malloc_sbuffer&) = delete;
        malloc_sbuffer& operator=(malloc_sbuffer&) = delete;
        malloc_sbuffer(malloc_sbuffer&&) = delete;
        malloc_sbuffer& operator=(malloc_sbuffer&&) = delete;
        ~malloc_sbuffer();
        
        friend class buffer_recycler;

#if defined(EBU_LIST_RECYCLE_SBUFFERS)
        const buffer_recycler_wptr recycler_;
#endif // defined(EBU_LIST_RECYCLE_SBUFFERS)

        const size_t size_ = 0;
        byte* const base_ = nullptr;
        std::atomic_int ref_count_ = 1;
    };

    using malloc_sbuffer_uptr = std::unique_ptr<malloc_sbuffer>;
}
