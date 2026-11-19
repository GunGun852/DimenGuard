#pragma once

#include "unit.hpp"
#include <compare>

namespace dimenguard {

// One double is the entire object. No runtime unit tag, virtual methods or heap.
// Construction from a raw number is explicit: the caller must choose its unit.
template<Unit U>
class quantity {
public:
    using unit_type = U;
    using dimension_type = typename U::dimension_type;
    using rep = double;

    constexpr quantity() noexcept = default;
    explicit constexpr quantity(double value) noexcept : value_(value) {}
    [[nodiscard]] constexpr double value() const noexcept { return value_; }

    template<Unit V> requires SameUnit<U, V>
    constexpr quantity& operator+=(quantity<V> rhs) noexcept {
        value_ += rhs.value(); return *this;
    }
    template<Unit V> requires SameUnit<U, V>
    constexpr quantity& operator-=(quantity<V> rhs) noexcept {
        value_ -= rhs.value(); return *this;
    }
    constexpr quantity& operator*=(double scalar) noexcept { value_ *= scalar; return *this; }
    constexpr quantity& operator/=(double scalar) noexcept { value_ /= scalar; return *this; }
private:
    double value_ = 0.0;
};

namespace detail {
template<class> struct is_quantity : std::false_type {};
template<Unit U> struct is_quantity<quantity<U>> : std::true_type {};
}
template<class Q>
concept Quantity = detail::is_quantity<std::remove_cvref_t<Q>>::value;

// There are deliberately no implicit conversions between different units.
template<Unit To, Unit From> requires SameDimension<To, From>
[[nodiscard]] constexpr quantity<To> unit_cast(quantity<From> input) noexcept {
    using factor = std::ratio_divide<typename From::scale, typename To::scale>;
    constexpr double multiplier = static_cast<double>(factor::num) /
                                  static_cast<double>(factor::den);
    return quantity<To>{input.value() * multiplier};
}

template<Unit U, Unit V> requires SameUnit<U, V>
[[nodiscard]] constexpr quantity<U> operator+(quantity<U> lhs, quantity<V> rhs) noexcept {
    return quantity<U>{lhs.value() + rhs.value()};
}
template<Unit U, Unit V> requires SameUnit<U, V>
[[nodiscard]] constexpr quantity<U> operator-(quantity<U> lhs, quantity<V> rhs) noexcept {
    return quantity<U>{lhs.value() - rhs.value()};
}
template<Unit U>
[[nodiscard]] constexpr quantity<U> operator-(quantity<U> q) noexcept {
    return quantity<U>{-q.value()};
}
template<Unit U>
[[nodiscard]] constexpr quantity<U> operator+(quantity<U> q) noexcept { return q; }

template<Unit U, Unit V>
[[nodiscard]] constexpr quantity<unit_product<U, V>>
operator*(quantity<U> lhs, quantity<V> rhs) noexcept {
    return quantity<unit_product<U, V>>{lhs.value() * rhs.value()};
}
template<Unit U, Unit V>
[[nodiscard]] constexpr quantity<unit_quotient<U, V>>
operator/(quantity<U> lhs, quantity<V> rhs) noexcept {
    return quantity<unit_quotient<U, V>>{lhs.value() / rhs.value()};
}
template<Unit U>
[[nodiscard]] constexpr quantity<U> operator*(quantity<U> q, double scalar) noexcept {
    return quantity<U>{q.value() * scalar};
}
template<Unit U>
[[nodiscard]] constexpr quantity<U> operator*(double scalar, quantity<U> q) noexcept { return q * scalar; }
template<Unit U>
[[nodiscard]] constexpr quantity<U> operator/(quantity<U> q, double scalar) noexcept {
    return quantity<U>{q.value() / scalar};
}
template<Unit U>
[[nodiscard]] constexpr quantity<unit_quotient<unit<dimensionless>, U>>
operator/(double scalar, quantity<U> q) noexcept {
    return quantity<unit_quotient<unit<dimensionless>, U>>{scalar / q.value()};
}

template<Unit U, Unit V> requires SameUnit<U, V>
[[nodiscard]] constexpr bool operator==(quantity<U> lhs, quantity<V> rhs) noexcept {
    return lhs.value() == rhs.value();
}
template<Unit U, Unit V> requires SameUnit<U, V>
[[nodiscard]] constexpr std::partial_ordering
operator<=>(quantity<U> lhs, quantity<V> rhs) noexcept { return lhs.value() <=> rhs.value(); }

template<int N, Unit U> requires (N >= -8 && N <= 8)
[[nodiscard]] constexpr quantity<unit_power<U, N>> pow(quantity<U> q) noexcept {
    double result = 1.0;
    for (int i = 0; i < (N < 0 ? -N : N); ++i) result *= q.value();
    if constexpr (N < 0) result = 1.0 / result;
    return quantity<unit_power<U, N>>{result};
}

} // namespace dimenguard
