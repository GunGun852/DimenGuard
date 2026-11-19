#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
constexpr auto speed(dimenguard::quantity<dimenguard::metre> d,
                     dimenguard::quantity<dimenguard::second> t) { return d / t; }
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = speed(10_m, 2_kg);
#else
    auto result = speed(10_m, 2_s);
#endif
    (void)result;
}
