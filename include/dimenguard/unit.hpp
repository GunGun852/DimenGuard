#pragma once

#include "dimension.hpp"
#include <ratio>

namespace dimenguard {

namespace detail {
template<class> struct is_ratio : std::false_type {};
template<std::intmax_t N, std::intmax_t D>
struct is_ratio<std::ratio<N, D>> : std::true_type {};
}

// Scale means "this many coherent SI units per one unit".
// A foot is unit<length, ratio<381,1250>>, i.e. exactly 0.3048 metres.
template<Dimension D, class Scale = std::ratio<1>>
    requires detail::is_ratio<Scale>::value
struct unit {
    static_assert(Scale::num > 0, "A unit's scale must be positive");
    using dimension_type = D;
    using scale = typename Scale::type; // Normalize equivalent ratios.
};

namespace detail {
template<class> struct is_unit : std::false_type {};
template<Dimension D, class R>
struct is_unit<unit<D, R>> : std::true_type {};
}

template<class U>
concept Unit = detail::is_unit<U>::value;

template<class U, class V>
concept SameDimension = Unit<U> && Unit<V> &&
    std::same_as<typename U::dimension_type, typename V::dimension_type>;

template<class U, class V>
concept SameUnit = SameDimension<U, V> &&
    std::ratio_equal_v<typename U::scale, typename V::scale>;

template<Unit U, Unit V>
using unit_product = unit<dimension_product<typename U::dimension_type, typename V::dimension_type>,
                          std::ratio_multiply<typename U::scale, typename V::scale>>;
template<Unit U, Unit V>
using unit_quotient = unit<dimension_quotient<typename U::dimension_type, typename V::dimension_type>,
                           std::ratio_divide<typename U::scale, typename V::scale>>;

namespace detail {
template<class R, unsigned N> struct ratio_power {
    using type = std::ratio_multiply<R, typename ratio_power<R, N - 1>::type>;
};
template<class R> struct ratio_power<R, 0> { using type = std::ratio<1>; };
template<class R, int N> struct signed_ratio_power {
    using base = std::conditional_t<(N < 0), std::ratio<R::den, R::num>, R>;
    using type = typename ratio_power<base, static_cast<unsigned>(N < 0 ? -N : N)>::type;
};
}

// Small integral powers keep this teaching implementation bounded and readable.
// std::ratio also requires intermediate ratios to fit std::intmax_t.
template<Unit U, int N> requires (N >= -8 && N <= 8)
using unit_power = unit<dimension<U::dimension_type::length * N,
                                 U::dimension_type::mass * N,
                                 U::dimension_type::time * N>,
                        typename detail::signed_ratio_power<typename U::scale, N>::type>;

} // namespace dimenguard
