#include "bisect/bimo/memory/oview.h"
#include "bisect/bimo/memory/sbuffer_factory.h"
#include "bisect/bimo/memory/memory.h"
#include "bisect/bimo/idioms/enforce.h"

using namespace bisect::bimo;
using namespace bisect;

//------------------------------------------------------------------------------
namespace
{
    ptrdiff_t get_size(const sbuffer* data)
    {
        return (data == nullptr) ? 0 : data->size();
    }

    auto get_view(const sbuffer* data, ptrdiff_t start_offset, ptrdiff_t valid_size)
    {
        if (data == nullptr) return cbyte_span();
        if (data->cbegin() == nullptr) return cbyte_span();
        return cbyte_span(data->cbegin() + start_offset, valid_size);
    }
    
    auto get_view(const sbuffer* data, ptrdiff_t start_offset)
    {
        return get_view(data, start_offset, get_size(data));
    }

    auto start_offset(const oview& s, const sbuffer& b)
    {
        return s.view().data() - b.cbegin();
    }
}
//------------------------------------------------------------------------------
#pragma region oview

owning_view::owning_view(sbuffer_ptr data)
    : data_(std::move(data)),
    view_(get_view(data_.get(), 0))
{
}

owning_view::owning_view(sbuffer_ptr data, ptrdiff_t start_offset, ptrdiff_t valid_size)
    : data_(std::move(data)),
    view_(get_view(data_.get(), start_offset, valid_size))
{
    BIMO_ASSERT(data_ || valid_size == 0);
    BIMO_ASSERT(data_ || start_offset == 0);
    BIMO_ASSERT(data_ == false || start_offset + valid_size <= data_->size());
 }

owning_view::owning_view(const owning_view& other)
: data_(other.data_),
view_(other.view_)
{
}

owning_view& owning_view::operator=(const owning_view& other)
{
    if(this == &other) return *this;

    this->data_ = other.data_;
    this->view_ = other.view_;
    return *this;
}

owning_view::owning_view(owning_view&&) = default;
owning_view& owning_view::operator=(owning_view&&) = default;

owning_view::operator const cbyte_span&() const
{
    return this->view();
}

const cbyte_span& owning_view::view() const
{
    return view_;
}

owning_view::operator bool() const
{
    return !view_.empty();
}

#pragma endregion owning_view

//------------------------------------------------------------------------------

#pragma region merge
//------------------------------------------------------------------------------
namespace
{
    owning_view merge_non_adjacent(sbuffer_factory& factory, owning_view&& first, owning_view&& second)
    {
        const auto total_size = size(first) + size(second);
        auto sbuffer = factory.get_buffer(total_size);
        BIMO_ASSERT(sbuffer);
        BIMO_ASSERT(sbuffer->size() >= total_size);

        copy(first.view(), gsl::make_span(sbuffer->begin(), sbuffer->end()));
        copy(second.view(), gsl::make_span(sbuffer->begin() + size(first), sbuffer->size() - size(first)));

        return owning_view(std::move(sbuffer), 0, total_size);
    }
}
//------------------------------------------------------------------------------

owning_view bimo::merge(sbuffer_factory& factory, owning_view&& first, owning_view&& second)
{
    if (size(first) == 0) return second;
    if (size(second) == 0) return first;

    const auto e1 = first.view().data() + size(first);
    const auto b2 = second.view().data();
    if (e1 == b2)
    {
        BIMO_ASSERT(first.data_);
        const auto so = start_offset(first, *first.data_);
        return owning_view(std::move(first.data_), so, size(first) + size(second));
    }
    else
    {
        return merge_non_adjacent(factory, std::move(first), std::move(second));
    }
}

#pragma endregion merge

std::tuple<owning_view, owning_view> bimo::split(owning_view&& source, ptrdiff_t split_offset)
{
    BIMO_ENFORCE(split_offset <= size(source), std::runtime_error, "trying to split past the end");
    BIMO_ASSERT(source.data_);
    const auto so = start_offset(source, *source.data_);
    owning_view left(source.data_, so, split_offset);
    owning_view right(source.data_, so + split_offset, size(source) - split_offset);

    return { left, right };
}
