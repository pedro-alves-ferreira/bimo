#pragma once

#include "bisect/bimo/types.h"
#include "bisect/bimo/memory/mapped_oview.h"
#include <array>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    template<typename Mapping, class Lens, typename ...LensArgs>
    class mapped_view
    {
    public:
        explicit mapped_view(oview&& data, LensArgs... args)
            : data_(std::move(data)),
            view_(*reinterpret_cast<const Mapping*>(data_.view().data()), args...)
        {
            BIMO_ASSERT(size(data_) >= sizeof(Mapping));
        }

        const Lens& view() const
        {
            return view_;
        }

        const Lens& operator()() const
        {
            return view_;
        }

    private:
        oview data_;
        Lens view_;
    };
}
