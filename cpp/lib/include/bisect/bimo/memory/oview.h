#pragma once

#include "bisect/bimo/memory/sbuffer_factory.h"
#include <tuple>

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    class owning_view;
    
    std::tuple<owning_view, owning_view> split(owning_view&& source, ptrdiff_t split_offset) noexcept;
    owning_view merge(sbuffer_factory& factory, owning_view&& first, owning_view&& second);

    // Based on ACE
    class owning_view
    {
    public:
        owning_view() = default;
        explicit owning_view(sbuffer_ptr data) noexcept;
        owning_view(sbuffer_ptr data, ptrdiff_t start_offset, ptrdiff_t valid_size) noexcept;

        owning_view(const owning_view&) noexcept;
        owning_view& operator=(const owning_view&) noexcept;

        owning_view(owning_view&&) noexcept;
        owning_view& operator=(owning_view&&) noexcept;

        const cbyte_span& view() const noexcept;

        operator const cbyte_span&() const noexcept;
        operator bool() const noexcept;

    private:
        mutable sbuffer_ptr data_;
        cbyte_span view_;

        friend std::tuple<owning_view, owning_view> split(owning_view&& source, ptrdiff_t split_offset) noexcept;
        friend owning_view merge(sbuffer_factory& factory, owning_view&& first, owning_view&& second);
    };

    inline const auto size(const owning_view& s)
    {
        return s.view().size();
    }

    inline const auto begin(const owning_view& s)
    {
        return s.view().begin();
    }

    inline const auto end(const owning_view& s)
    {
        return s.view().end();
    }

    using oview = owning_view;
}
