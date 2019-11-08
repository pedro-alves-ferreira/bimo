#pragma once

#include "bisect/bimo/memory/mapped_oview.h"
#include "bisect/bimo/types.h"
#include <array>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    template <typename Mapping, class Lens, typename... LensArgs> class mapped_view
    {
      public:
        explicit mapped_view(oview&& data, LensArgs... args)
            : data_(std::move(data)), view_(*reinterpret_cast<const Mapping*>(data_.view().data()), args...)
        {
            BIMO_ASSERT(size(data_) >= ssizeof<Mapping>());
        }

        const Lens& view() const { return view_; }

        const Lens& operator()() const { return view_; }

        const oview& data() const { return data_; }

      private:
        oview data_;
        Lens view_;
    };
} // namespace bisect::bimo
