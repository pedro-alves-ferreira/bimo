#include "pch.h"

#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "counting_sbuffer_factory.h"

using namespace bisect::bimo;
using namespace bisect::bimo::test;

//------------------------------------------------------------------------------

namespace bisect::bimo::test
{
    struct callback_delegate : public buffer_recycler
    {
        std::function<void()> on_recycle;

        callback_delegate(std::function<void()> _on_recycle);

        void recycle(malloc_sbuffer*) override;
    };
} // namespace bisect::bimo::test

callback_delegate::callback_delegate(std::function<void()> _on_recycle) : on_recycle(_on_recycle)
{
}

void callback_delegate::recycle(malloc_sbuffer*)
{
    on_recycle();
}

//------------------------------------------------------------------------------

counting_sbuffer_factory::counting_sbuffer_factory()
    : callback_(std::make_shared<callback_delegate>([this]() { this->on_recycle(); }))
{
}

sbuffer_ptr counting_sbuffer_factory::get_buffer(size_t buffer_size)
{
    ++create_counter_;
    return {new malloc_sbuffer(buffer_size, callback_)};
}

int counting_sbuffer_factory::created_count() const
{
    return create_counter_;
}

int counting_sbuffer_factory::outstanding_count() const
{
    assert(create_counter_ >= recycled_counter_);
    return create_counter_ - recycled_counter_;
}

void counting_sbuffer_factory::on_recycle()
{
    ++recycled_counter_;
}