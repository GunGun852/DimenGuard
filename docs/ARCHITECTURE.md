# Architecture

DimenGuard separates a small compile-time numeric core from optional presentation.

| Header | Responsibility | Runtime data/cost |
|---|---|---|
| `dimension.hpp` | Integer template exponents and algebra | No per-quantity data |
| `unit.hpp` | Dimensions plus normalized positive rational scales | No per-quantity data |
| `quantity.hpp` | Value storage, constrained operators and explicit casts | One double and required arithmetic |
| `si.hpp` | Named aliases and literals | Literal conversion to double |
| `format.hpp` | Symbols and stream output | Strings/output when requested |
| `diagnostics.hpp` | Type-policy analysis and educational explanations | `analyze`: compile time; `explain`: runtime strings |

## Example: deriving force

1. `100_m` is `quantity<unit<dimension<1,0,0>, ratio<1>>>`.
2. `10_s` is `quantity<unit<dimension<0,0,1>, ratio<1>>>`.
3. Dividing produces exponents `(1,0,0) - (0,0,1) = (1,0,-1)`.
4. Dividing the speed by `5_s` produces `(1,0,-2)`.
5. Multiplying by `2_kg`, whose exponents are `(0,1,0)`, produces `(1,1,-2)`.
6. The canonical unit is `newton`; the stored double is 4.

The unit aliases name template specializations rather than introducing incompatible
wrapper classes for each named unit. As a result, a derived force can be passed to a
function accepting `quantity<newton>` without manually converting its type.

## Why explicit conversions?

Two same-dimension quantities may use different scales. `SameUnit` deliberately
requires equivalent scale ratios for addition and comparison. The caller decides
the output unit through `unit_cast`; the compiler still checks that the requested
target dimension is compatible. This is a conservative, easily explained policy.

For multiplication and division, scale ratios travel in the result type. Values
remain in that derived unit until explicitly converted. No automatic SI normalization
or runtime unit registry is needed for core arithmetic.

## CLI boundary

The command-line converter must handle strings supplied at runtime. Its finite unit
registry constructs a `std::variant` of known quantity types. A visitor selects
`unit_cast` only for compatible types using `if constexpr`. A wrong user-entered
unit name/dimension raises a runtime error; an invalid C++ expression is rejected
at compile time. The two kinds of validation are intentionally documented separately.

## Why explanations do not compile invalid arithmetic

The educational API inspects operand unit types without evaluating `a + b`.
This lets a working demonstration explain why a source expression would be rejected.
Actual compile-fail fixtures independently demonstrate the compiler rejection.

## Ownership and extensibility

The core has value semantics and no allocation. Optional formatting owns temporary
strings through standard RAII. A downstream project can add `using inch =
unit<length, ratio<127,5000>>` without modifying the library. Its quantities then
participate in the same constrained arithmetic and explanation API.
