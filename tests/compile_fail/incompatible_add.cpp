#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = 1_m + 1_s; // Invalid: length and time are different dimensions.
#else
    auto result = 1_m + 1_m;
#endif
    (void)result;
}
