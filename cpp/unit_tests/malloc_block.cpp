#include "pch.h"

#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/memory/memory.h"
#include "catch2/catch.hpp"

using namespace bisect::bimo;

//------------------------------------------------------------------------------

SCENARIO("Blocks have valid iterators")
{
    GIVEN("a sbuffer with some data")
    {
        constexpr auto data = to_byte_array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

        auto b = sbuffer_ptr(new malloc_sbuffer(10, {}));
        REQUIRE(b);

        copy(data, gsl::make_span(b->begin(), b->size()));

        WHEN("we ckeck the iterators")
        {
            THEN("they are valid")
            {
                REQUIRE(b->begin() != nullptr);
                REQUIRE(b->end() != nullptr);
            }
        }

        WHEN("we ckeck the iterators' span")
        {
            THEN("it is correct")
            {
                REQUIRE(b->end() - b->begin() == ptrdiff_t(10));
            }
        }

        WHEN("we ckeck the values")
        {
            THEN("they are correct")
            {
                const auto bg = *b->begin();
                REQUIRE(bg == byte(0));
                auto end = b->end();
                --end;
                REQUIRE(*end == byte(9));
            }
        }
    }

    GIVEN("a sbuffer with length 0")
    {
        auto b = sbuffer_ptr(new malloc_sbuffer(0, {}));

        WHEN("we ckeck the iterators")
        {
            THEN("they are nullptr")
            {
                REQUIRE(b->begin() == nullptr);
                REQUIRE(b->end() == nullptr);
            }
        }

        WHEN("we ckeck begin and end")
        {
            THEN("they are equal")
            {
                REQUIRE(b->begin() == b->end());
            }
        }

        WHEN("we ckeck size")
        {
            THEN("it is 0")
            {
                REQUIRE(b->size() == 0);
            }
        }
    }
}
