#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = 1_m + 2.0; // What unit does 2.0 have? The caller must say.
#else
    auto result = 1_m + 2_m;
#endif
    (void)result;
}
