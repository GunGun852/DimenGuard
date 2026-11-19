#include <dimenguard/si.hpp>
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    dimenguard::quantity<dimenguard::unit<dimenguard::dimension<33, 0, 0>>> result{1.0};
#else
    dimenguard::quantity<dimenguard::unit<dimenguard::dimension<32, 0, 0>>> result{1.0};
#endif
    (void)result;
}
