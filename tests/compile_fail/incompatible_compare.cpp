#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    bool result = 1_m < 1_s;
#else
    bool result = 1_m < 2_m;
#endif
    (void)result;
}
