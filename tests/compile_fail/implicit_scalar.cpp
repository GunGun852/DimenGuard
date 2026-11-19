#include <dimenguard/si.hpp>
using namespace dimenguard::literals;
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    double result = 1_m; // Units cannot disappear implicitly.
#else
    double result = (1_m).value();
#endif
    (void)result;
}
