#pragma once

#include "bisect/bimo/memory/shared_buffer.h"

namespace bisect::bimo::test
{
    class sbuffer_mock : public sbuffer
    {
    public:
        sbuffer_mock(std::function<void()> on_deleted = []() {}, ptrdiff_t size = 0);

        byte* begin() noexcept override;
        byte* end() noexcept override;
        const byte* cbegin() const noexcept override;
        const byte* cend() const noexcept override;
        ptrdiff_t size() const noexcept override;

        void add_ref() noexcept override;
        void remove_ref() noexcept override;

        int ref_count() const noexcept;

    private:
        ~sbuffer_mock();

        byte * const data_ = nullptr;
        byte* const end_ = nullptr;
        int ref_count_ = 1;
        std::function<void()> on_deleted_;
    };
}
