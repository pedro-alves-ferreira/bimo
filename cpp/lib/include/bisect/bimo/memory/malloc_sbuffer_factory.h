#pragma once

#include "bisect/bimo/memory/sbuffer_factory.h"
#include "bisect/bimo/memory/malloc_sbuffer.h"
#include <unordered_map>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class malloc_sbuffer;

    class malloc_sbuffer_factory : public sbuffer_factory
    {
    public:
        malloc_sbuffer_factory();

        sbuffer_ptr get_buffer(size_t buffer_size) override;

    private:
        struct impl;
        const std::shared_ptr<impl> impl_;
    };
}
