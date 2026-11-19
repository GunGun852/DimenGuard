#pragma once

#include "format.hpp"
#include <string>

namespace dimenguard {

enum class operation { add, subtract, multiply, divide };
enum class verdict { valid, dimension_mismatch, explicit_conversion_required };

template<operation Op, Unit Left, Unit Right>
[[nodiscard]] consteval verdict analyze() {
    if constexpr (Op == operation::add || Op == operation::subtract) {
        if constexpr (!SameDimension<Left, Right>) return verdict::dimension_mismatch;
        else if constexpr (!SameUnit<Left, Right>) return verdict::explicit_conversion_required;
    }
    return verdict::valid;
}

struct diagnostic {
    verdict status;
    std::string left_dimension;
    std::string right_dimension;
    std::string result_dimension;
    std::string explanation;
    std::string suggestion;
    [[nodiscard]] bool accepted() const noexcept { return status == verdict::valid; }
};

// This optional educational layer allocates strings at runtime. The quantity
// arithmetic above has no dependency on it and does not carry a trace around.
// We inspect types here, so an invalid expression can be explained without
// making the entire demonstration program fail to compile.
template<operation Op, Unit Left, Unit Right>
[[nodiscard]] diagnostic explain() {
    using L = typename Left::dimension_type;
    using R = typename Right::dimension_type;
    constexpr auto status = analyze<Op, Left, Right>();
    diagnostic result{status, dimension_string<L>(), dimension_string<R>(), "", "", ""};
    if constexpr (status == verdict::dimension_mismatch) {
        result.explanation = "Addition/subtraction requires identical dimensions; these exponent vectors differ.";
        result.suggestion = "Check the formula or the declared unit. unit_cast cannot convert length to time.";
    } else if constexpr (status == verdict::explicit_conversion_required) {
        result.result_dimension = dimension_string<L>();
        result.explanation = "Dimensions match, but unit scales differ. This library requires explicit conversion.";
        result.suggestion = "Convert one operand with unit_cast to the other operand's unit, then add/subtract.";
    } else if constexpr (Op == operation::multiply) {
        result.result_dimension = dimension_string<dimension_product<L, R>>();
        result.explanation = "Multiplication adds the L, M, T exponents component by component.";
    } else if constexpr (Op == operation::divide) {
        result.result_dimension = dimension_string<dimension_quotient<L, R>>();
        result.explanation = "Division subtracts the right-hand L, M, T exponents from the left-hand ones.";
    } else {
        result.result_dimension = dimension_string<L>();
        result.explanation = "Dimensions and scales match. Addition/subtraction preserves the dimension.";
    }
    return result;
}

} // namespace dimenguard
