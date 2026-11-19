#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    auto result = dimenguard::unit_cast<dimenguard::second>(1_m);
#else
    auto result = dimenguard::unit_cast<dimenguard::centimetre>(1_m);
#endif
    (void)result;
}
