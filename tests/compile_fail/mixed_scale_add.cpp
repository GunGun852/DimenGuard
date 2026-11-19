#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = 1_m + 1_ft; // Valid physical dimensions, but conversion must be explicit.
#else
    auto result = 1_m + dimenguard::unit_cast<dimenguard::metre>(1_ft);
#endif
    (void)result;
}
