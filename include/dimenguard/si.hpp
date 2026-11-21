#pragma once

#include "quantity.hpp"

namespace dimenguard {

using one = unit<dimensionless>;
using metre = unit<length>;
using centimetre = unit<length, std::centi>;
using kilometre = unit<length, std::kilo>;
using foot = unit<length, std::ratio<381, 1250>>;
// International definitions: 1 yd = 0.9144 m; 1 mi = 1609.344 m.
using yard = unit<length, std::ratio<1143, 1250>>;
using mile = unit<length, std::ratio<201168, 125>>;
using kilogram = unit<mass>;
using gram = unit<mass, std::milli>;
using second = unit<time>;
using millisecond = unit<time, std::milli>;
using minute = unit<time, std::ratio<60>>;
using hour = unit<time, std::ratio<3600>>;
using square_metre = unit<area>;
using cubic_metre = unit<volume>;
using metres_per_second = unit<velocity>;
using kilometres_per_hour = unit<velocity, std::ratio<5, 18>>;
using metres_per_second_squared = unit<acceleration>;
using newton = unit<force>;

// Literals return quantities, not raw doubles. Integer literals use double too.
namespace literals {
#define DIMENGUARD_LITERAL(suffix, unit_name) \
    [[nodiscard]] constexpr quantity<unit_name> operator"" suffix(long double v) noexcept { \
        return quantity<unit_name>{static_cast<double>(v)}; \
    } \
    [[nodiscard]] constexpr quantity<unit_name> operator"" suffix(unsigned long long v) noexcept { \
        return quantity<unit_name>{static_cast<double>(v)}; \
    }
DIMENGUARD_LITERAL(_m, metre)
DIMENGUARD_LITERAL(_cm, centimetre)
DIMENGUARD_LITERAL(_km, kilometre)
DIMENGUARD_LITERAL(_ft, foot)
DIMENGUARD_LITERAL(_yd, yard)
DIMENGUARD_LITERAL(_mi, mile)
DIMENGUARD_LITERAL(_kg, kilogram)
DIMENGUARD_LITERAL(_g, gram)
DIMENGUARD_LITERAL(_s, second)
DIMENGUARD_LITERAL(_ms, millisecond)
DIMENGUARD_LITERAL(_min, minute)
DIMENGUARD_LITERAL(_h, hour)
DIMENGUARD_LITERAL(_N, newton)
#undef DIMENGUARD_LITERAL
}

} // namespace dimenguard
