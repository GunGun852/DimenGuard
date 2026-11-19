#include <dimenguard/si.hpp>

namespace dg = dimenguard;

// Function arguments are runtime inputs. Both versions perform the SAME math.
// Kept externally visible so an optimizer cannot remove unused test functions.
extern "C" double raw_speed(double distance, double duration) {
    return distance / duration;
}
extern "C" double typed_speed(double distance, double duration) {
    return (dg::quantity<dg::metre>{distance} / dg::quantity<dg::second>{duration}).value();
}
extern "C" double raw_force(double mass, double speed, double duration) {
    return mass * (speed / duration);
}
extern "C" double typed_force(double mass, double speed, double duration) {
    return (dg::quantity<dg::kilogram>{mass} *
            (dg::quantity<dg::metres_per_second>{speed} / dg::quantity<dg::second>{duration})).value();
}
extern "C" double raw_feet_to_metres(double feet) { return feet * (381.0 / 1250.0); }
extern "C" double typed_feet_to_metres(double feet) {
    return dg::unit_cast<dg::metre>(dg::quantity<dg::foot>{feet}).value();
}
