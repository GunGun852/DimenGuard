#include <dimenguard/dimenguard.hpp>
#include <iostream>

namespace dg = dimenguard;

using inch = dg::unit<dg::length, std::ratio<127, 5000>>;
using inch_per_second = dg::unit_quotient<inch, dg::second>;

int main() {
    constexpr dg::quantity<inch> ruler{12.0};
    constexpr auto feet = dg::unit_cast<dg::foot>(ruler);
    static_assert(feet.value() == 1.0);
    std::cout << "12 inches = " << feet << '\n';
    const auto speed = ruler / dg::quantity<dg::second>{2.0};
    std::cout << "12 inches / 2 seconds = "
              << dg::unit_cast<dg::metres_per_second>(speed) << '\n';
}
