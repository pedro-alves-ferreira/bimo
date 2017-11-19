#pragma once

#include "bisect/bimo/memory/shared_buffer.h"
#include <memory>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class sbuffer_factory
    {
    public:
        virtual ~sbuffer_factory() = default;

        virtual sbuffer_ptr get_buffer(size_t buffer_size) = 0;
    };

    using sbuffer_factory_ptr = std::shared_ptr<sbuffer_factory>;
}
