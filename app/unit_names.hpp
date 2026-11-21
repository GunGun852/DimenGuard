#pragma once

#include <array>
#include <ostream>
#include <string_view>

namespace dimenguard::cli {

// Display names only: the CLI registry remains the source of supported symbols.
constexpr std::string_view unit_full_name(std::string_view symbol) {
    struct entry { std::string_view symbol; std::string_view name; };
    constexpr std::array<entry, 19> names{{
        {"m", "metre"},
        {"cm", "centimetre"},
        {"km", "kilometre"},
        {"ft", "foot"},
        {"yd", "yard"},
        {"mi", "mile"},
        {"kg", "kilogram"},
        {"g", "gram"},
        {"s", "second"},
        {"ms", "millisecond"},
        {"min", "minute"},
        {"h", "hour"},
        {"m^2", "square metre"},
        {"m^3", "cubic metre"},
        {"m/s", "metre per second"},
        {"km/h", "kilometre per hour"},
        {"m/s^2", "metre per second squared"},
        {"N", "newton"},
        {"1", "dimensionless"}
    }};
    for (const auto& unit : names) {
        if (unit.symbol == symbol) return unit.name;
    }
    return {};
}

inline void print_unit_label(std::ostream& out, std::string_view symbol) {
    out << symbol;
    if (const auto name = unit_full_name(symbol); !name.empty()) {
        out << " (" << name << ')';
    }
}

} // namespace dimenguard::cli
