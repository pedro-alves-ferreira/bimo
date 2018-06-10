#pragma once

#include <memory>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class malloc_sbuffer;

    class buffer_recycler
    {
    protected:
        void delete_sbuffer(malloc_sbuffer* b);

    public:
        virtual ~buffer_recycler() = default;

        virtual void recycle(malloc_sbuffer* buffer) = 0;
    };

    using buffer_recycler_ptr = std::shared_ptr<buffer_recycler>;
    using buffer_recycler_wptr = std::weak_ptr<buffer_recycler>;
}
