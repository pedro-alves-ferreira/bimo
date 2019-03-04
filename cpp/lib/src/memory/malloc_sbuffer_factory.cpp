#include "bisect/bimo/memory/malloc_sbuffer_factory.h"
#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/memory/buffer_recycler.h"
#include <mutex>
using namespace bisect::bimo;

//------------------------------------------------------------------------------
#if defined(EBU_LIST_RECYCLE_SBUFFERS)

namespace
{
    constexpr size_t alignment_size = 256;
}

struct malloc_sbuffer_factory::impl : public buffer_recycler
{
public:
    sbuffer_ptr get_buffer(ptrdiff_t buffer_size, buffer_recycler_ptr self)
    {
        {
            std::unique_lock<std::mutex> lock(m_);

            const auto it = cache_.lower_bound(buffer_size);
            if (it != cache_.end() && it->second->size() >= buffer_size)
            {
                assert(it->second->size() >= buffer_size);
                auto to_return = std::move(it->second);
                cache_.erase(it);

                return sbuffer_ptr(to_return.release());
            }
        }

        return sbuffer_ptr(new malloc_sbuffer(align_size(buffer_size), self));
    }

private:
    void recycle(malloc_sbuffer* buffer) override
    {
        std::unique_lock<std::mutex> lock(m_);
        cache_.insert({ buffer->size(), std::unique_ptr<malloc_sbuffer>(buffer) });
    }

    size_t align_size(size_t requested_size)
    {
        const auto remainder = requested_size % alignment_size;
        const auto count = requested_size / alignment_size;

        if (remainder == 0)
        {
            return requested_size;
        }

        return (count + 1) * alignment_size;
    }

    std::mutex m_;
    std::unordered_multimap<size_t, std::unique_ptr<malloc_sbuffer>> cache_;
};

#else // defined(EBU_LIST_RECYCLE_SBUFFERS)

struct malloc_sbuffer_factory::impl : public buffer_recycler
{
public:
    void recycle(malloc_sbuffer* buffer) override
    {
        delete_sbuffer(buffer);
    }

    size_t align_size(size_t requested_size)
    {
        return requested_size;
    }

    sbuffer_ptr get_buffer(size_t buffer_size, buffer_recycler_ptr self)
    {
        return sbuffer_ptr(new malloc_sbuffer(align_size(buffer_size), self));
    }
};

#endif // defined(EBU_LIST_RECYCLE_SBUFFERS)

//------------------------------------------------------------------------------

malloc_sbuffer_factory::malloc_sbuffer_factory()
    : impl_(std::make_shared<impl>())
{
}

sbuffer_ptr malloc_sbuffer_factory::get_buffer(size_t buffer_size)
{
    return impl_->get_buffer(buffer_size, impl_);
}

