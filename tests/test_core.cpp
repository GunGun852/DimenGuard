#include <dimenguard/dimenguard.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

namespace dg = dimenguard;
using namespace dg::literals;

template<class A, class B> concept Addable = requires(A a, B b) { a + b; };
template<class A, class B> concept Comparable = requires(A a, B b) { a < b; a == b; };
template<class To, class From> concept Castable = requires(From q) { dg::unit_cast<To>(q); };
template<class Q, int N> concept Powerable = requires(Q q) { dg::pow<N>(q); };

using M = dg::quantity<dg::metre>;
using S = dg::quantity<dg::second>;
using CM = dg::quantity<dg::centimetre>;
using EquivalentMetre = dg::unit<dg::length, std::ratio<100, 100>>;

static_assert(dg::Dimension<dg::length> && !dg::Dimension<int>);
static_assert(dg::Unit<dg::metre> && !dg::Unit<double>);
static_assert(dg::Quantity<M> && dg::Quantity<const M&> && !dg::Quantity<double>);
static_assert(sizeof(M) == sizeof(double));
static_assert(alignof(M) == alignof(double));
static_assert(std::is_trivially_copyable_v<M> && std::is_standard_layout_v<M>);
static_assert(!std::is_convertible_v<double, M> && !std::is_convertible_v<M, double>);
static_assert(!std::is_convertible_v<CM, M>);
static_assert(Addable<M, M> && !Addable<M, S> && !Addable<M, CM> && !Addable<M, double>);
static_assert(Comparable<M, M> && !Comparable<M, S> && !Comparable<M, CM>);
static_assert(Castable<dg::metre, CM> && !Castable<dg::second, M>);
static_assert(Powerable<M, 2> && Powerable<M, -1> && !Powerable<M, 9>);
static_assert(dg::SameUnit<EquivalentMetre, dg::metre>);
static_assert(dg::SameDimension<dg::metre, dg::foot> && !dg::SameUnit<dg::metre, dg::foot>);
static_assert(std::same_as<decltype(1.0_m / 1.0_s), dg::quantity<dg::metres_per_second>>);
static_assert(std::same_as<decltype(1.0_kg * (1.0_m / 1.0_s / 1.0_s)), dg::quantity<dg::newton>>);
static_assert(std::same_as<decltype(1.0_m / 1.0_m), dg::quantity<dg::one>>);
static_assert(std::same_as<decltype(dg::pow<2>(1.0_m)), dg::quantity<dg::square_metre>>);
static_assert((100.0_m / 10.0_s).value() == 10.0);
static_assert(dg::unit_cast<dg::metre>(1.0_km).value() == 1000.0);
static_assert(dg::unit_cast<dg::kilogram>(500.0_g).value() == 0.5);
static_assert(dg::pow<0>(0.0_m).value() == 1.0);
static_assert(dg::pow<-1>(2.0_s).value() == 0.5);
static_assert(dg::analyze<dg::operation::add, dg::metre, dg::second>() == dg::verdict::dimension_mismatch);
static_assert(dg::analyze<dg::operation::add, dg::metre, dg::foot>() == dg::verdict::explicit_conversion_required);
static_assert(dg::analyze<dg::operation::multiply, dg::metre, dg::second>() == dg::verdict::valid);

namespace {
int checks = 0;
int failures = 0;
void check(bool result, const char* expression, int line) {
    ++checks;
    if (!result) { ++failures; std::cerr << "FAIL line " << line << ": " << expression << '\n'; }
}
#define CHECK(...) check((__VA_ARGS__), #__VA_ARGS__, __LINE__)
bool near(double a, double b) {
    return std::abs(a - b) <= 1e-11 * std::max({1.0, std::abs(a), std::abs(b)});
}
}

int main() {
    CHECK(M{}.value() == 0.0);
    CHECK((2_m + 3_m).value() == 5.0);
    CHECK((2_m - 3_m).value() == -1.0);
    CHECK((-2_m).value() == -2.0);
    CHECK((+2_m).value() == 2.0);
    CHECK((2_m * 3.0).value() == 6.0);
    CHECK((3.0 * 2_m).value() == 6.0);
    CHECK((2_m / 4.0).value() == 0.5);
    CHECK((1.0 / 2_s).value() == 0.5);
    CHECK((3_m * 4_m).value() == 12.0);
    CHECK(dg::pow<3>(2_m).value() == 8.0);
    CHECK(dg::pow<-2>(2_s).value() == 0.25);
    CHECK(dg::unit_cast<dg::one>(1_km / 1_m).value() == 1000.0);
    CHECK((1_m + dg::quantity<EquivalentMetre>{2.0}).value() == 3.0);
    auto mutable_length = 10_m;
    mutable_length += 5_m; mutable_length -= 3_m; mutable_length *= 2.0; mutable_length /= 4.0;
    CHECK(mutable_length == 6_m);
    CHECK(1_m < 2_m && 2_m >= 2_m && 2_m != 3_m && 3_m > 2_m);
    CHECK(near(dg::unit_cast<dg::metre>(1_ft).value(), 0.3048));
    CHECK(near((1_m + dg::unit_cast<dg::metre>(1_ft)).value(), 1.3048));
    CHECK(near(dg::unit_cast<dg::foot>(1_m).value(), 3.280839895013123));
    CHECK(dg::unit_cast<dg::second>(1_h).value() == 3600.0);
    CHECK(dg::unit_cast<dg::second>(1_min).value() == 60.0);
    CHECK(dg::unit_cast<dg::second>(500_ms).value() == 0.5);
    CHECK(dg::unit_cast<dg::metre>(200_cm).value() == 2.0);
    CHECK(near(dg::unit_cast<dg::metres_per_second>(72_km / 1_h).value(), 20.0));
    CHECK(near(dg::unit_cast<dg::kilometres_per_hour>(20_m / 1_s).value(), 72.0));
    CHECK(near(dg::unit_cast<dg::square_metre>(3_cm * 4_cm).value(), 0.0012));
    CHECK(near(dg::unit_cast<dg::newton>(500_g * (100_cm / 1_s / 1_s)).value(), 0.5));
    CHECK(dg::dimension_string<dg::force>() == "L^1 M^1 T^-2");
    CHECK(dg::unit_symbol<dg::newton>() == "N");
    CHECK(dg::unit_symbol<dg::unit<dg::dimensionless, std::ratio<1000>>>() == "[1000/1] 1");
    CHECK(dg::unit_symbol<dg::unit_quotient<dg::foot, dg::second>>() == "[381/1250] m s^-1");
    std::ostringstream output; output << 3_m << ", " << 2_kg * (1_m / 1_s / 1_s);
    CHECK(output.str() == "3 m, 2 N");

    const auto mismatch = dg::explain<dg::operation::add, dg::metre, dg::second>();
    CHECK(!mismatch.accepted() && mismatch.result_dimension.empty());
    CHECK(mismatch.suggestion.find("cannot convert length to time") != std::string::npos);
    const auto convert = dg::explain<dg::operation::subtract, dg::metre, dg::foot>();
    CHECK(convert.status == dg::verdict::explicit_conversion_required);
    CHECK(convert.result_dimension == "L^1 M^0 T^0");
    const auto add = dg::explain<dg::operation::add, dg::metre, dg::metre>();
    CHECK(add.accepted() && add.result_dimension == add.left_dimension);
    const auto multiply = dg::explain<dg::operation::multiply, dg::kilogram, dg::metres_per_second_squared>();
    CHECK(multiply.accepted() && multiply.result_dimension == "L^1 M^1 T^-2");
    const auto divide = dg::explain<dg::operation::divide, dg::metre, dg::second>();
    CHECK(divide.accepted() && divide.result_dimension == "L^1 M^0 T^-1");

    // Floating-point semantics are preserved; dimensional safety is not range safety.
    const M nan{std::numeric_limits<double>::quiet_NaN()};
    CHECK(!(nan == nan) && !(nan < 0_m));
    CHECK((nan <=> 0_m) == std::partial_ordering::unordered);
    CHECK(std::signbit(M{-0.0}.value()));
    CHECK(std::isinf((1_m / 0.0).value()));

    // Deterministic samples compared with independent numerical conversion formulas.
    std::uint32_t state = 0x4d475244u;
    for (int i = 0; i < 2000; ++i) {
        state = state * 1664525u + 1013904223u;
        const double x = (static_cast<double>(state) / 4294967295.0 - 0.5) * 1e6;
        const double duration = 0.125 + static_cast<double>(i % 97);
        const double mass = 0.5 + static_cast<double>(i % 31);
        const auto metres = dg::unit_cast<dg::metre>(dg::quantity<dg::foot>{x});
        CHECK(near(metres.value(), x * 0.3048));
        CHECK(near(dg::unit_cast<dg::foot>(metres).value(), x));
        const auto speed = dg::quantity<dg::metre>{x} / dg::quantity<dg::second>{duration};
        CHECK(near(speed.value(), x / duration));
        CHECK(near(dg::unit_cast<dg::kilometres_per_hour>(speed).value(), x / duration * 3.6));
        const auto force = dg::quantity<dg::kilogram>{mass} * (speed / dg::quantity<dg::second>{duration});
        CHECK(near(force.value(), mass * (x / duration / duration)));
    }
    std::cout << checks << " runtime checks; " << failures << " failures. Compile-time assertions also passed.\n";
    return failures == 0 ? 0 : 1;
}
