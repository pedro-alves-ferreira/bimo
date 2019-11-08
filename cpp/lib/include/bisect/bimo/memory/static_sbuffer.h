#pragma once

#include "bisect/bimo/memory/shared_buffer.h"

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    template <int N> class static_sbuffer : public sbuffer
    {
      public:
        static_sbuffer(std::array<byte, N> data) : data_(data) {}

        byte* begin() noexcept override { return data_.data(); }
        byte* end() noexcept override { return data_.data() + data_.size(); }
        const byte* cbegin() const noexcept override { return data_.data(); }
        const byte* cend() const noexcept override { return data_.data() + data_.size(); }
        ptrdiff_t size() const noexcept override { return data_.size(); }

        void add_ref() noexcept override {}
        void remove_ref() noexcept override {}

      private:
        std::array<byte, N> data_;
    };

    template <size_t N> sbuffer_ptr make_static_sbuffer(std::array<byte, N> data)
    {
        return new static_sbuffer<N>(data);
    }
} // namespace bisect::bimo
