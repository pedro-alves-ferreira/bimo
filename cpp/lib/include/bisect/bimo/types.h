#pragma once

#pragma warning(push)
#pragma warning(disable : 4996)
#include "gsl/gsl"
#pragma warning(pop)

//------------------------------------------------------------------------------

namespace bisect::bimo
{
    using byte       = gsl::byte;
    using byte_span  = gsl::span<byte>;
    using cbyte_span = gsl::span<const byte>;

    template <class T> constexpr byte check_cast_to_byte(T t)
    {
        // TODO: it is causing a compiler error
        // static_assert(t >= 0 && t <= 255, "byte only has 8 bits of storage, values must be in range 0-255");
        return static_cast<byte>(t);
    }

    template <class... Ts> constexpr auto to_byte_array(Ts... t)
    {
        return std::array<byte, sizeof...(Ts)>{check_cast_to_byte(t)...};
    }

    namespace details
    {
        template <class D, class... Types>
        using return_type = std::array<typename std::common_type<Types...>::type, sizeof...(Types)>;
    }

    template <class D = void, class... Types> constexpr details::return_type<D, Types...> make_array(Types&&... t)
    {
        return {std::forward<Types>(t)...};
    }

    template <typename T> constexpr ptrdiff_t ssizeof() { return static_cast<ptrdiff_t>(sizeof(T)); }
} // namespace bisect::bimo
