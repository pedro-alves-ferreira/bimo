#pragma once

#include "bisect/bimo/memory/sbuffer_factory.h"

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class malloc_sbuffer;

    class malloc_sbuffer_factory : public sbuffer_factory
    {
      public:
        malloc_sbuffer_factory();

        sbuffer_ptr get_buffer(size_t buffer_size) final;

      private:
        struct impl;
        const std::shared_ptr<impl> impl_;
    };
} // namespace bisect::bimo
