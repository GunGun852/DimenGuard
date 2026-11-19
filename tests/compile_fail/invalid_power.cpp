#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = dimenguard::pow<9>(2_m); // Outside the documented -8...8 range.
#else
    auto result = dimenguard::pow<2>(2_m);
#endif
    (void)result;
}
