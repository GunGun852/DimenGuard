#include <dimenguard/dimenguard.hpp>
#include <iostream>

namespace dg = dimenguard;

using inch = dg::unit<dg::length, std::ratio<127, 5000>>;
using inch_per_second = dg::unit_quotient<inch, dg::second>;

int main() {
    constexpr dg::quantity<inch> ruler{12.0};
    constexpr auto feet = dg::unit_cast<dg::foot>(ruler);
    static_assert(feet.value() == 1.0);

    constexpr dg::quantity<dg::second> duration{2.0};
    constexpr inch_per_second speed = ruler / duration;
    constexpr auto speed_mps = dg::unit_cast<dg::metres_per_second>(speed);

    std::cout << "12 inches = " << feet << '\n'
              << "12 inches / 2 seconds = " << speed_mps << '\n';

    return 0;
}
