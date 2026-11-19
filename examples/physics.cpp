#include <dimenguard/dimenguard.hpp>
#include <iostream>

namespace dg = dimenguard;
using namespace dg::literals;

// A reusable function cannot accidentally receive a mass as its duration.
constexpr dg::quantity<dg::newton> required_force(
    dg::quantity<dg::kilogram> mass,
    dg::quantity<dg::metres_per_second> final_speed,
    dg::quantity<dg::second> duration) {
    return mass * (final_speed / duration);
}

int main() {
    const auto mass = 1200.0_kg;
    const auto target = dg::unit_cast<dg::metres_per_second>(72.0_km / 1.0_h);
    std::cout << "Idealized constant acceleration (starting from rest):\n"
              << "Mass: " << mass << "\nTarget speed: " << target
              << "\nDuration: " << 10.0_s << "\nForce: "
              << required_force(mass, target, 10.0_s) << '\n';
    // Simplified teaching model: ignores resistance, changing mass and traction.
}
