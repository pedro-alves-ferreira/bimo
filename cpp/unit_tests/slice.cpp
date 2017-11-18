#include "pch.h"

#include "bisect/bimo/memory/malloc_sbuffer.h"
#include "bisect/bimo/memory/malloc_sbuffer_factory.h"
#include "bisect/bimo/memory/oview.h"
#include "bisect/bimo/memory/memory.h"

#include "catch.hpp"

#include <algorithm>
using namespace bisect::bimo;

//------------------------------------------------------------------------------
class counting_sbuffer_factory : public malloc_sbuffer_factory
{
    int counter_ = 0;

public:
    sbuffer_ptr get_buffer(size_t buffer_size) override
    {
        ++counter_;
        return malloc_sbuffer_factory::get_buffer(buffer_size);
    }

    int count() const { return counter_; }
};
//------------------------------------------------------------------------------

SCENARIO("A oview can have a null sbuffer")
{
    GIVEN("a oview constructed with an empty sbuffer")
    {
        auto s = oview(sbuffer_ptr());

        WHEN("we ckeck the iterators' distance")
        {
            THEN("it is correct")
            {
                auto span = s.view();
                REQUIRE(span.end() - span.begin() == ptrdiff_t(0));
            }
        }
    }
}

SCENARIO("Slices have valid iterators")
{
    GIVEN("a default constructed oview")
    {
        auto s = oview();

        WHEN("we ckeck the iterators' distance")
        {
            THEN("it is correct")
            {
                auto span = s.view();
                REQUIRE(span.end() - span.begin() == ptrdiff_t(0));
            }
        }
    }

    GIVEN("an empty oview")
    {
        auto s = oview(make_sbuffer<malloc_sbuffer>(1), 0, 0);

        WHEN("we ckeck the iterators' distance")
        {
            THEN("it is correct")
            {
                auto span = s.view();
                REQUIRE(span.end() - span.begin() == ptrdiff_t(0));
            }
        }
    }

    GIVEN("a oview with some data")
    {
        constexpr auto data = to_byte_array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

        auto b = make_sbuffer<malloc_sbuffer>(10);
        copy(data, gsl::make_span(b->begin(), b->size()));

        auto s = oview(std::move(b));

        WHEN("we ckeck the iterators' distance")
        {
            THEN("it is correct")
            {
                auto span = s.view();
                REQUIRE(span.end() - span.begin() == ptrdiff_t(10));
            }
        }

        WHEN("we ckeck the values")
        {
            THEN("they are correct")
            {
                auto span = s.view();
                const auto bg = *span.begin();
                REQUIRE(bg == byte(0));
                auto end = span.end();
                --end;
                REQUIRE(*end == byte(9));

                REQUIRE(equal(span, data));
            }
        }
    }
}

SCENARIO("Slices have an implicit conversion operator to a span")
{
    constexpr auto data = to_byte_array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

    auto b = make_sbuffer<malloc_sbuffer>(10);
    copy(data, gsl::make_span(b->begin(), b->size()));

    GIVEN("a oview with some data")
    {
        auto s = oview(std::move(b));

        WHEN("we convert to span")
        {
            THEN("it has valid iterators")
            {
                cbyte_span s2 = s;
                REQUIRE(s2.begin() != cbyte_span::iterator());
                REQUIRE(s2.end() != cbyte_span::iterator());
            }
        }
    }
}

SCENARIO("Slices can be split")
{
    GIVEN("a oview with some data")
    {
        constexpr auto data = to_byte_array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);

        auto b = make_sbuffer<malloc_sbuffer>(10);
        copy(data, gsl::make_span(b->begin(), b->size()));

        auto s = oview(std::move(b));

        WHEN("we split it in the middle")
        {
            auto[s1, s2] = split(std::move(s), 5);
            THEN("get two correct oviews")
            {
                REQUIRE(s1.view().size() == 5);
                REQUIRE(s2.view().size() == 5);

                const auto d1 = gsl::make_span(&data[0], 5);
                const auto d2 = gsl::make_span(&data[5], 5);
                REQUIRE(equal(d1, s1));
                REQUIRE(equal(d2, s2));
            }
        }
    }

    GIVEN("an empty oview")
    {
        auto s = oview(make_sbuffer<malloc_sbuffer>(1), 0, 0);

        WHEN("we split it at 0")
        {
            auto[s1, s2] = split(std::move(s), 0);
            THEN("get two empty oviews")
            {
                REQUIRE(s1.view().size() == 0);
                REQUIRE(s2.view().size() == 0);
            }
        }
    }
}

SCENARIO("Slices can be merged")
{
    GIVEN("two oviews with some data")
    {
        malloc_sbuffer_factory f;

        constexpr auto data1 = to_byte_array(0, 1, 2, 3, 4);
        auto b1 = make_sbuffer<malloc_sbuffer>(5);
        copy(data1, as_span(*b1));

        constexpr auto data2 = to_byte_array(5, 6, 7, 8, 9);
        auto b2 = make_sbuffer<malloc_sbuffer>(5);
        copy(data2, as_span(*b2));

        auto s1 = oview(std::move(b1));
        auto s2 = oview(std::move(b2));

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("we get a pair of oviews pointing to the correct data")
            {
                const auto sr1 = gsl::make_span(&s.view()[0], 5);
                const auto sr2 = gsl::make_span(&s.view()[5], 5);
                REQUIRE(equal(sr1, data1));
                REQUIRE(equal(sr2, data2));
            }
        }
    }

    GIVEN("two adjacent oviews")
    {
        counting_sbuffer_factory f;

        auto b = make_sbuffer<malloc_sbuffer>(10);
        auto s1 = oview(b, 0, 5);
        auto s2 = oview(b, 5, 5);

        const auto s1_begin = &s1.view()[0];
        const auto s2_last = &s2.view()[4];

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("we don't reallocate")
            {
                REQUIRE(f.count() == 0);

                REQUIRE(s.view().size() == 10);

                REQUIRE(&s.view()[0] == s1_begin);
                REQUIRE(&s.view()[9] == s2_last);
            }
        }
    }

    GIVEN("one empty and a non-empty oview")
    {
        malloc_sbuffer_factory f;

        constexpr auto data = to_byte_array(0, 1, 2, 3, 4);
        auto b = make_sbuffer<malloc_sbuffer>(5);
        copy(data, as_span(*b));

        auto s1 = oview(b, 0, 0);
        auto s2 = oview(b, 0, 5);

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("the result is correct")
            {
                REQUIRE(s.view().size() == 5);
                REQUIRE(equal(s, data));
            }
        }
    }

    GIVEN("one non-empty and a empty oview")
    {
        malloc_sbuffer_factory f;

        constexpr auto data = to_byte_array(0, 1, 2, 3, 4);
        auto b = make_sbuffer<malloc_sbuffer>(5);
        copy(data, as_span(*b));

        auto s1 = oview(b, 0, 5);
        auto s2 = oview(b, 0, 0);

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("the result is correct")
            {
                REQUIRE(s.view().size() == 5);
                REQUIRE(equal(s, data));
            }
        }
    }

    GIVEN("one default constructed and a non-empty oview")
    {
        malloc_sbuffer_factory f;

        constexpr auto data = to_byte_array(0, 1, 2, 3, 4);
        auto b = make_sbuffer<malloc_sbuffer>(5);
        copy(data, as_span(*b));

        auto s1 = oview();
        auto s2 = oview(b, 0, 5);

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("the result is correct")
            {
                REQUIRE(s.view().size() == 5);
                REQUIRE(equal(s, data));
            }
        }
    }

    GIVEN("one non-empty and a default constructed oview")
    {
        malloc_sbuffer_factory f;

        constexpr auto data = to_byte_array(0, 1, 2, 3, 4);
        auto b = make_sbuffer<malloc_sbuffer>(5);
        copy(data, as_span(*b));

        auto s1 = oview(b, 0, 5);
        auto s2 = oview();

        WHEN("we merge them")
        {
            auto s = merge(f, std::move(s1), std::move(s2));
            THEN("the result is correct")
            {
                REQUIRE(s.view().size() == 5);
                REQUIRE(equal(s, data));
            }
        }
    }
}
