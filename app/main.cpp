#include <dimenguard/dimenguard.hpp>

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <variant>

namespace dg = dimenguard;
using namespace dg::literals;

namespace {
using any_quantity = std::variant<dg::quantity<dg::metre>, dg::quantity<dg::centimetre>,
    dg::quantity<dg::kilometre>, dg::quantity<dg::foot>, dg::quantity<dg::kilogram>,
    dg::quantity<dg::gram>, dg::quantity<dg::second>, dg::quantity<dg::millisecond>,
    dg::quantity<dg::minute>, dg::quantity<dg::hour>, dg::quantity<dg::square_metre>,
    dg::quantity<dg::cubic_metre>, dg::quantity<dg::metres_per_second>,
    dg::quantity<dg::kilometres_per_hour>, dg::quantity<dg::metres_per_second_squared>,
    dg::quantity<dg::newton>, dg::quantity<dg::one>>;

template<dg::Unit U> any_quantity make_quantity(double value) { return dg::quantity<U>{value}; }
struct registry_entry { std::string_view symbol; any_quantity (*make)(double); };
constexpr std::array<registry_entry, 17> registry{{
    registry_entry{"m", make_quantity<dg::metre>}, {"cm", make_quantity<dg::centimetre>},
    {"km", make_quantity<dg::kilometre>}, {"ft", make_quantity<dg::foot>},
    {"kg", make_quantity<dg::kilogram>}, {"g", make_quantity<dg::gram>},
    {"s", make_quantity<dg::second>}, {"ms", make_quantity<dg::millisecond>},
    {"min", make_quantity<dg::minute>}, {"h", make_quantity<dg::hour>},
    {"m^2", make_quantity<dg::square_metre>}, {"m^3", make_quantity<dg::cubic_metre>},
    {"m/s", make_quantity<dg::metres_per_second>}, {"km/h", make_quantity<dg::kilometres_per_hour>},
    {"m/s^2", make_quantity<dg::metres_per_second_squared>}, {"N", make_quantity<dg::newton>},
    {"1", make_quantity<dg::one>}}};

any_quantity lookup(std::string_view symbol, double value) {
    for (const auto& entry : registry) if (entry.symbol == symbol) return entry.make(value);
    throw std::invalid_argument("Unknown unit '" + std::string(symbol) + "'. Check the supported unit symbols.");
}

double parse_value(const std::string& input) {
    std::size_t consumed = 0;
    double value = 0.0;
    try {
        value = std::stod(input, &consumed);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("VALUE must be a finite number with no trailing characters");
    } catch (const std::out_of_range&) {
        throw std::out_of_range("VALUE is outside the supported number range");
    }
    if (consumed != input.size() || !std::isfinite(value))
        throw std::invalid_argument("VALUE must be a finite number with no trailing characters");
    return value;
}

void convert_and_print(double value, std::string_view from_symbol, std::string_view to_symbol) {
    const auto source = lookup(from_symbol, value);
    const auto target = lookup(to_symbol, 0.0);
    const double converted = std::visit([](auto from, auto to) -> double {
        using From = typename decltype(from)::unit_type;
        using To = typename decltype(to)::unit_type;
        if constexpr (dg::SameDimension<From, To>) return dg::unit_cast<To>(from).value();
        else throw std::invalid_argument("Incompatible dimensions: cannot convert " +
            dg::unit_symbol<From>() + " to " + dg::unit_symbol<To>());
    }, source, target);
    if (!std::isfinite(converted)) throw std::out_of_range("Conversion exceeds finite double range");
    std::cout << value << ' ' << from_symbol << " = " << converted << ' ' << to_symbol << '\n';
}

void list_units() {
    for (const auto& entry : registry) std::cout << entry.symbol << '\n';
}

void show_diagnostic(std::string_view expression, const dg::diagnostic& d) {
    std::cout << "\n" << expression << "\n"
              << "  Left:   " << d.left_dimension << "\n"
              << "  Right:  " << d.right_dimension << "\n"
              << "  Status: " << (d.accepted() ? "VALID" : "REJECTED") << "\n";
    if (!d.result_dimension.empty()) std::cout << "  Result: " << d.result_dimension << "\n";
    std::cout << "  " << d.explanation << "\n";
    if (!d.suggestion.empty()) std::cout << "  Fix: " << d.suggestion << "\n";
}

void explanation_demo() {
    std::cout << "DIMENSION LAB | exponent order: Length, Mass, Time\n";
    show_diagnostic("distance / time -> velocity",
        dg::explain<dg::operation::divide, dg::metre, dg::second>());
    show_diagnostic("velocity / time -> acceleration",
        dg::explain<dg::operation::divide, dg::metres_per_second, dg::second>());
    show_diagnostic("mass * acceleration -> force",
        dg::explain<dg::operation::multiply, dg::kilogram, dg::metres_per_second_squared>());
    show_diagnostic("1 metre + 1 second",
        dg::explain<dg::operation::add, dg::metre, dg::second>());
    show_diagnostic("1 metre + 1 foot (before explicit conversion)",
        dg::explain<dg::operation::add, dg::metre, dg::foot>());
    std::cout << "\nThis explanation tool inspects types. Real compile-fail tests are in tests/compile_fail/.\n";
}

void demo() {
    constexpr auto speed = 100.0_m/10.0_s;
    constexpr auto acceleration = speed/5.0_s;
    constexpr auto force = 2.0_kg * acceleration;
    static_assert(std::same_as<typename decltype(force)::dimension_type, dg::force>);
    static_assert(force.value() == 4.0);

    std::cout << "\nDIMENGUARD 1.0.0 | Compile-time units, clear formulas\n\n"
              << "  100 m / 10 s         = " << speed << "\n"
              << "  (100 m / 10 s) / 5 s = " << acceleration << "\n"
              << "  2 kg * acceleration  = " << force << "\n"
              << "  1 m + cast<m>(1 ft)  = " << (1.0_m + dg::unit_cast<dg::metre>(1.0_ft)) << "\n"
              << "  72 km/h in m/s       = "
              << dg::unit_cast<dg::metres_per_second>(72.0_km / 1.0_h) << "\n"
              << "  Area of 3 m x 4 m    = " << 3.0_m * 4.0_m << "\n\n"
              << "  sizeof(quantity<metre>) = " << sizeof(dg::quantity<dg::metre>) << " bytes\n"
              << "  sizeof(double)          = " << sizeof(double) << " bytes\n\n"
              << "Try: dimenguard explain\n"
              << "Try: dimenguard convert 72 km/h m/s\n"
              << "Invalid examples are separate source files: run the compile-fail tests.\n";
}

// Read one answer per line so bad numbers cannot leave std::cin in a failed state.
bool read_answer(std::string_view prompt, std::string& answer) {
    std::cout << prompt << std::flush;
    if (!std::getline(std::cin, answer)) {
        std::cout << "\nGoodbye!\n";
        return false;
    }
    const auto first = answer.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) answer.clear();
    else answer = answer.substr(first, answer.find_last_not_of(" \t\r\n") - first + 1);
    return true;
}

void interactive_menu() {
    std::cout << "\nWelcome to DimenGuard!\n"
              << "Enter each answer, then press Enter. Unit symbols are case-sensitive.\n";
    while (true) {
        std::cout << "\n1. Convert units\n"
                  << "2. List supported units\n"
                  << "3. Show physics demo\n"
                  << "4. Explain dimensions\n"
                  << "0. Exit\n\n";
        std::string choice;
        if (!read_answer("Choose an option: ", choice)) return;
        if (choice == "0") { std::cout << "Goodbye!\n"; return; }
        if (choice == "2") { list_units(); continue; }
        if (choice == "3") { demo(); continue; }
        if (choice == "4") { explanation_demo(); continue; }
        if (choice != "1") {
            std::cout << "Please choose 0, 1, 2, 3 or 4.\n";
            continue;
        }
        try {
            std::string value_text, from_symbol, to_symbol;
            if (!read_answer("Enter value: ", value_text)) return;
            const double value = parse_value(value_text);
            if (!read_answer("From unit (e.g. m, ft, kg, km/h): ", from_symbol)) return;
            if (!read_answer("To unit: ", to_symbol)) return;
            convert_and_print(value, from_symbol, to_symbol);
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\nPlease try again.\n";
        }
    }
}

void help() {
    std::cout << "DimenGuard 1.0.0\n"
              << "Usage:\n"
              << "  dimenguard                     Open the interactive menu\n"
              << "  dimenguard interactive         Open the interactive menu\n"
              << "  dimenguard demo                Show typed physics calculations\n"
              << "  dimenguard explain             Explain dimension algebra and rejected types\n"
              << "  dimenguard units               List supported CLI unit symbols\n"
              << "  dimenguard convert VALUE FROM TO\n"
              << "  dimenguard --version\n"
              << "Examples: convert 1 ft m | convert 72 km/h m/s | convert 500 g kg\n"
              << "The CLI checks user-entered unit names at runtime; the C++ API checks types at compile time.\n";
}
}

int main(int argc, char** argv) {
    try {
        std::cout << std::setprecision(12);
        if (argc == 1) { interactive_menu(); return 0; }
        if (argc == 2) {
            const std::string_view command = argv[1];
            if (command == "interactive") { interactive_menu(); return 0; }
            if (command == "demo") { demo(); return 0; }
            if (command == "explain") { explanation_demo(); return 0; }
            if (command == "--help" || command == "help" || command == "-h") { help(); return 0; }
            if (command == "--version") { std::cout << "DimenGuard 1.0.0\n"; return 0; }
            if (command == "units") { list_units(); return 0; }
        }
        if (argc == 5 && std::string_view(argv[1]) == "convert") {
            convert_and_print(parse_value(argv[2]), argv[3], argv[4]);
            return 0;
        }
        help();
        return 2;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 2;
    }
}
