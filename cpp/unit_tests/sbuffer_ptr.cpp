#include "pch.h"

#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/memory/memory.h"
#include "bisect/bimo/memory/shared_buffer.h"
#include "catch2/catch.hpp"
#include "counting_sbuffer_factory.h"
#include "sbuffer_mock.h"

#include <algorithm>
using namespace bisect::bimo;
using namespace bisect::bimo::test;

//------------------------------------------------------------------------------

SCENARIO("sbuffer_ptrs can be copy assigned")
{
    GIVEN("one sbuffer_ptr with some data")
    {
        WHEN("we assign it an empty sbuffer_ptr")
        {
            THEN("the buffer must have been released")
            {
                auto deleted_count = 0;
                auto on_delete     = [&]() { ++deleted_count; };

                counting_sbuffer_factory f;

                {
                    auto b = sbuffer_ptr(new test::sbuffer_mock(on_delete, 5));

                    auto c = sbuffer_ptr(new test::sbuffer_mock(on_delete, 5));

                    b = c;
                }

                REQUIRE(deleted_count == 2);
                REQUIRE(f.outstanding_count() == 0);
            }
        }
    }
}

SCENARIO("sbuffer_ptrs can be move assigned")
{
    GIVEN("one sbuffer_ptr with some data")
    {
        WHEN("we move assign it an empty sbuffer_ptr")
        {
            THEN("the buffer must have been released")
            {
                auto deleted_count = 0;
                auto on_delete     = [&]() { ++deleted_count; };

                counting_sbuffer_factory f;

                {
                    auto b = sbuffer_ptr(new test::sbuffer_mock(on_delete, 5));

                    b = sbuffer_ptr{};
                }

                REQUIRE(deleted_count == 1);
                REQUIRE(f.outstanding_count() == 0);
            }
        }
    }
}
