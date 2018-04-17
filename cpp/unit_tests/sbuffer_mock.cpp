#include "pch.h"

#include "sbuffer_mock.h"

using namespace bisect::bimo;
using namespace bisect::bimo::test;

//------------------------------------------------------------------------------

sbuffer_mock::sbuffer_mock(std::function<void()> on_deleted, ptrdiff_t size)
    : data_(size == 0 ? nullptr : reinterpret_cast<byte*>(::malloc(size))),
    end_(size == 0 ? nullptr : data_ + size),
    on_deleted_(on_deleted)
{
}

sbuffer_mock::~sbuffer_mock() 
{ 
    ::free(data_);
}

byte* sbuffer_mock::begin() noexcept { return data_; }
byte* sbuffer_mock::end() noexcept { return end_; }
const byte* sbuffer_mock::cbegin() const noexcept { return data_; }
const byte* sbuffer_mock::cend() const noexcept { return end_; }
ptrdiff_t sbuffer_mock::size() const noexcept { return end_ - data_; }

void sbuffer_mock::add_ref() noexcept { ++ref_count_; }
void sbuffer_mock::remove_ref() noexcept
{
    if (--ref_count_ == 0)
    {
        on_deleted_();
        delete this;
    }
}

int sbuffer_mock::ref_count() const noexcept { return ref_count_; }
