#include <dimenguard/si.hpp>
int main() {
#ifdef DIMENGUARD_NEGATIVE_CASE
    dimenguard::quantity<dimenguard::unit<dimenguard::length, std::ratio<0>>> result{1.0};
#else
    dimenguard::quantity<dimenguard::unit<dimenguard::length, std::ratio<1>>> result{1.0};
#endif
    (void)result;
}
