#pragma once

#include "bisect/bimo/memory/shared_buffer.h"
#include "bisect/bimo/memory/sbuffer_factory.h"

namespace bisect::bimo::test
{
    struct callback_delegate;

    class counting_sbuffer_factory : public sbuffer_factory
    {
    public:
        counting_sbuffer_factory();

        sbuffer_ptr get_buffer(size_t buffer_size) override;

        int created_count() const;

		// This is only valid if buffers are recycled
        int outstanding_count() const;

    private:
        void on_recycle();
        int create_counter_ = 0;
        int recycled_counter_ = 0;
        std::shared_ptr<callback_delegate> callback_;
    };
}
