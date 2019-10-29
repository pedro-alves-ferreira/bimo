#include "pch.h"

#include "bisect/bimo/memory/static_sbuffer.h"
#include "catch2/catch.hpp"
#include <functional>

using namespace bisect::bimo;

//------------------------------------------------------------------------------
class simple_sbuffer : public sbuffer
{
    int ref_count_ = 1;

public:
    simple_sbuffer() = default;
    ~simple_sbuffer() = default;

    byte* begin() noexcept override { return nullptr; }
    byte* end() noexcept override { return nullptr; }
    const byte* cbegin() const noexcept override { return nullptr; }
    const byte* cend() const noexcept override { return nullptr; }
    ptrdiff_t size() const noexcept override { return 0; }

    void add_ref() noexcept override { ++ref_count_; }
    void remove_ref() noexcept override { --ref_count_; }

    int ref_count() const noexcept { return ref_count_; }
};

//------------------------------------------------------------------------------

SCENARIO("Null sbuffers")
{
    GIVEN("a nullptr to a sbuffer")
    {
        WHEN("we pass it to a sbuffer_ptr")
        {
            sbuffer_ptr bp(nullptr);

            THEN("the accessor return nullptr")
            {
                REQUIRE(bp.get() == nullptr);
            }
        }
    }
    GIVEN("a sbuffer_ptr")
    {
        WHEN("we default construct it")
        {
            sbuffer_ptr bp;

            THEN("the accessor return nullptr")
            {
                REQUIRE(bp.get() == nullptr);
            }
        }
    }
}

SCENARIO("Conversion to bool")
{
    auto b = std::make_unique<simple_sbuffer>();

    GIVEN("a sbuffer_ptr")
    {
        sbuffer_ptr bp(b.get());

        WHEN("we it is not empty")
        {
            THEN("it converts to true")
            {
                REQUIRE(bp == true);
            }
        }

        WHEN("we it is empty")
        {
            bp = sbuffer_ptr();

            THEN("it converts to false")
            {
                REQUIRE(bp == false);
            }
        }
    }
}

SCENARIO("Conversion to sbuffer")
{
    auto b = std::make_unique<simple_sbuffer>();

    GIVEN("a sbuffer_ptr")
    {
        sbuffer_ptr bp(b.get());

        WHEN("we use operator ->")
        {
            THEN("it converts to sbuffer")
            {
                REQUIRE(bp.operator->() == b.get());
            }
        }

        WHEN("we use operator *")
        {
            THEN("it converts to sbuffer")
            {
                REQUIRE(&bp.operator*() == b.get());
            }
        }
    }
}

SCENARIO("Simple ref count")
{
    GIVEN("a sbuffer")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we pass it to a sbuffer_ptr")
        {
            THEN("it decrements the reference count upon destruction")
            {
                sbuffer_ptr bp(b.get());
                REQUIRE(b->ref_count() == 1);
            }
            REQUIRE(b->ref_count() == 0);
        }
    }

}

SCENARIO("Self assignment")
{
    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we assign it to itself")
        {

            {
                sbuffer_ptr bp1(b.get());

                bp1 = bp1;
                THEN("it still points to the same sbuffer")
                {
                    REQUIRE(bp1.get() == b.get());
                }
                THEN("it does not increment the sbuffer's reference count")
                {
                    REQUIRE(b->ref_count() == 1);
                }
            }

            THEN("it decrements the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }

    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we move assign it to itself")
        {
            {
                sbuffer_ptr bp1(b.get());

                bp1 = std::move(bp1);
                THEN("it still points to the same sbuffer")
                {
                    REQUIRE(bp1.get() == b.get());
                }
                THEN("it does not increment the sbuffer's reference count")
                {
                    REQUIRE(b->ref_count() == 1);
                }
            }

            THEN("it decrements the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }
}

SCENARIO("Copy construction and assignment")
{
    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we copy it to another sbuffer_ptr")
        {
            {
                sbuffer_ptr bp1(b.get());

                {
                    sbuffer_ptr bp2(bp1);
                    THEN("both point to the same sbuffer")
                    {
                        REQUIRE(bp1.get() == bp2.get());
                    }
                    THEN("it increments the sbuffer's reference count")
                    {
                        REQUIRE(b->ref_count() == 2);
                    }
                }

                THEN("the second one, decrements the reference count upon destruction")
                {
                    REQUIRE(b->ref_count() == 1);
                }
            }

            THEN("the first one, decrements the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }

    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we assign it to another sbuffer_ptr")
        {
            {
                sbuffer_ptr bp1(b.get());
                {

                    sbuffer_ptr bp2;
                    bp2 = bp1;

                    THEN("both point to the same sbuffer")
                    {
                        REQUIRE(bp1.get() == bp2.get());
                    }
                    THEN("it increments the sbuffer's reference count")
                    {
                        REQUIRE(b->ref_count() == 2);
                    }
                }

                THEN("the second one, decrements the reference count upon destruction")
                {
                    REQUIRE(b->ref_count() == 1);
                }
            }

            THEN("the first one, decrements the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }
}

SCENARIO("Move construction and assignment")
{
    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we move it to another sbuffer_ptr")
        {
            sbuffer_ptr bp1(b.get());

            {
                {
                    sbuffer_ptr bp2(std::move(bp1));

                    THEN("the new one points to the same sbuffer")
                    {
                        REQUIRE(bp2.get() == b.get());
                    }
                    THEN("the old one points to nullptr")
                    {
                        REQUIRE(bp1.get() == nullptr);
                    }
                }

                THEN("the second one, decrements the reference count upon destruction")
                {
                    REQUIRE(b->ref_count() == 0);
                }
            }

            THEN("the first one, does not decrement the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }

    GIVEN("a sbuffer_ptr")
    {
        auto b = std::make_unique<simple_sbuffer>();

        WHEN("we move it to another sbuffer_ptr")
        {
            {
                sbuffer_ptr bp1(b.get());

                {
                    sbuffer_ptr bp2;
                    bp2 = std::move(bp1);

                    THEN("the new one points to the same sbuffer")
                    {
                        REQUIRE(bp2.get() == b.get());
                    }
                    THEN("the old one points to nullptr")
                    {
                        REQUIRE(bp1.get() == nullptr);
                    }
                }

                THEN("the second one, decrements the reference count upon destruction")
                {
                    REQUIRE(b->ref_count() == 0);
                }
            }

            THEN("the first one, does not decrement the reference count upon destruction")
            {
                REQUIRE(b->ref_count() == 0);
            }
        }
    }
}