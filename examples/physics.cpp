#include <dimenguard/dimenguard.hpp>
#include <iostream>

namespace dg = dimenguard;
using namespace dg::literals;

constexpr dg::quantity<dg::newton> required_force(
    dg::quantity<dg::kilogram> mass,
    dg::quantity<dg::metres_per_second> final_speed,
    dg::quantity<dg::second> duration) noexcept {
    return mass * (final_speed / duration);
}

int main() {
    constexpr auto mass = 1200.0_kg;
    constexpr auto target_speed = dg::unit_cast<dg::metres_per_second>(72.0_km / 1.0_h);
    constexpr auto duration = 10.0_s;
    constexpr auto force = required_force(mass, target_speed, duration);

    std::cout << "Idealized constant acceleration (starting from rest):\n"
              << "Mass: " << mass << '\n'
              << "Target speed: " << target_speed << '\n'
              << "Duration: " << duration << '\n'
              << "Force: " << force << '\n';

    return 0;
}
