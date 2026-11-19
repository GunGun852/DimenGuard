#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    dimenguard::quantity<dimenguard::metre> result = 1_ft;
#else
    dimenguard::quantity<dimenguard::metre> result = dimenguard::unit_cast<dimenguard::metre>(1_ft);
#endif
    (void)result;
}
