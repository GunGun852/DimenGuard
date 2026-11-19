#pragma once

#include "si.hpp"
#include <ostream>
#include <string>

namespace dimenguard {

template<Dimension D>
[[nodiscard]] std::string dimension_string() {
    return "L^" + std::to_string(D::length) + " M^" + std::to_string(D::mass) +
           " T^" + std::to_string(D::time);
}

template<Unit U>
[[nodiscard]] std::string unit_symbol() {
    if constexpr (SameUnit<U, one>) return "1";
    else if constexpr (SameUnit<U, metre>) return "m";
    else if constexpr (SameUnit<U, centimetre>) return "cm";
    else if constexpr (SameUnit<U, kilometre>) return "km";
    else if constexpr (SameUnit<U, foot>) return "ft";
    else if constexpr (SameUnit<U, kilogram>) return "kg";
    else if constexpr (SameUnit<U, gram>) return "g";
    else if constexpr (SameUnit<U, second>) return "s";
    else if constexpr (SameUnit<U, millisecond>) return "ms";
    else if constexpr (SameUnit<U, minute>) return "min";
    else if constexpr (SameUnit<U, hour>) return "h";
    else if constexpr (SameUnit<U, square_metre>) return "m^2";
    else if constexpr (SameUnit<U, cubic_metre>) return "m^3";
    else if constexpr (SameUnit<U, metres_per_second>) return "m/s";
    else if constexpr (SameUnit<U, kilometres_per_hour>) return "km/h";
    else if constexpr (SameUnit<U, metres_per_second_squared>) return "m/s^2";
    else if constexpr (SameUnit<U, newton>) return "N";
    else {
        using D = typename U::dimension_type;
        using R = typename U::scale;
        std::string result;
        if constexpr (R::num != R::den)
            result = "[" + std::to_string(R::num) + "/" + std::to_string(R::den) + "] ";
        auto append = [&result](const char* name, int exponent) {
            if (exponent == 0) return;
            if (!result.empty() && result.back() != ' ') result += ' ';
            result += name;
            if (exponent != 1) result += '^' + std::to_string(exponent);
        };
        append("kg", D::mass); append("m", D::length); append("s", D::time);
        if constexpr (std::same_as<D, dimensionless>) result += "1";
        return result;
    }
}

template<Unit U>
std::ostream& operator<<(std::ostream& stream, quantity<U> q) {
    return stream << q.value() << ' ' << unit_symbol<U>();
}

} // namespace dimenguard
