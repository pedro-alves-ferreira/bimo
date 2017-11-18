#pragma once

#include "bisect/bimo/memory/oview.h"

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    template<typename MappedStruct>
    class mapped_oview
    {
    public:
        explicit mapped_oview(oview&& data)
            : data_(std::move(data))
        {
            BIMO_ASSERT(size(data_) >= sizeof(MappedStruct));
        }

        const MappedStruct& value() const
        {
            return *reinterpret_cast<const MappedStruct*>(data_.view().data());
        }

    private:
        oview data_;
    };
}
