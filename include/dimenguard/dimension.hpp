#pragma once

#include <concepts>
#include <type_traits>

namespace dimenguard {

// Integers are non-type template parameters: these exponents are type information,
// not data stored in each quantity. Order is Length, Mass, Time throughout.
template<int L, int M, int T>
struct dimension {
    static_assert(L >= -32 && L <= 32 && M >= -32 && M <= 32 && T >= -32 && T <= 32,
                  "DimenGuard supports dimension exponents from -32 to 32");
    static constexpr int length = L;
    static constexpr int mass = M;
    static constexpr int time = T;
};

namespace detail {
template<class> struct is_dimension : std::false_type {};
template<int L, int M, int T>
struct is_dimension<dimension<L, M, T>> : std::true_type {};
}

template<class D>
concept Dimension = detail::is_dimension<D>::value &&
    (D::length >= -32 && D::length <= 32 && D::mass >= -32 && D::mass <= 32 &&
     D::time >= -32 && D::time <= 32);

template<Dimension A, Dimension B>
using dimension_product = dimension<A::length + B::length,
                                    A::mass + B::mass, A::time + B::time>;
template<Dimension A, Dimension B>
using dimension_quotient = dimension<A::length - B::length,
                                     A::mass - B::mass, A::time - B::time>;

using dimensionless = dimension<0, 0, 0>;
using length = dimension<1, 0, 0>;
using mass = dimension<0, 1, 0>;
using time = dimension<0, 0, 1>;
using area = dimension<2, 0, 0>;
using volume = dimension<3, 0, 0>;
using velocity = dimension<1, 0, -1>;
using acceleration = dimension<1, 0, -2>;
using force = dimension<1, 1, -2>;

} // namespace dimenguard
