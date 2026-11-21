# API reference

Namespace: `dimenguard`. Literal namespace: `dimenguard::literals`.
All core numeric operations below are `constexpr` and `noexcept`.

## Types

| Name | Meaning |
|---|---|
| `dimension<L, M, T>` | Integer exponents in Length, Mass, Time order; each from -32 to 32 |
| `unit<D, Ratio>` | A dimension and a positive rational scale relative to coherent SI units |
| `quantity<U>` | One explicitly constructed `double`, expressed in unit `U` |
| `Dimension<D>`, `Unit<U>`, `Quantity<Q>` | Concepts recognizing the library's types |
| `SameDimension<U,V>` | Identical dimension vectors |
| `SameUnit<U,V>` | Same dimension and equivalent normalized scale ratios |

These concepts recognize the provided templates, rather than arbitrary types with
similarly named members. Extend through `unit<...>` aliases, as in the custom-unit example.

## Built-in units and literals

| Unit type | Dimension | Scale to coherent SI | Literal |
|---|---|---|---|
| `metre` | Length | 1 | `_m` |
| `centimetre` | Length | 1/100 | `_cm` |
| `kilometre` | Length | 1000 | `_km` |
| `foot` | Length | 381/1250 | `_ft` |
| `yard` | Length | 1143/1250 | `_yd` |
| `mile` | Length | 201168/125 | `_mi` |
| `kilogram` | Mass | 1 | `_kg` |
| `gram` | Mass | 1/1000 | `_g` |
| `second` | Time | 1 | `_s` |
| `millisecond` | Time | 1/1000 | `_ms` |
| `minute` | Time | 60 | `_min` |
| `hour` | Time | 3600 | `_h` |
| `newton` | Force | 1 | `_N` |

Also available: `one`, `square_metre`, `cubic_metre`, `metres_per_second`,
`kilometres_per_hour`, and `metres_per_second_squared`. Construct these with
`quantity<U>{value}` or derive them through arithmetic. For example, `72_km / 1_h`
has the same unit type as `quantity<kilometres_per_hour>`.

Integer and floating literals both store a `double`. Integer literals above its
exact integer range may round. A type-level exact rational scale does not make
floating-point computations exact.

`yard` and `mile` use international definitions: 1 yd = 0.9144 m and
1 mi = 1609.344 m = 1760 yd = 5280 ft. See the
[NIST conversion factors](https://www.nist.gov/pml/special-publication-811/nist-guide-si-appendix-b-conversion-factors).
The console accepts the case-sensitive symbols `yd` and `mi`.

```cpp
using namespace dimenguard::literals;
auto distance = dimenguard::unit_cast<dimenguard::metre>(1_mi); // 1609.344 m
auto length = 1_yd + dimenguard::unit_cast<dimenguard::yard>(3_ft); // 2 yd
auto speed = dimenguard::unit_cast<dimenguard::metres_per_second>(60_mi / 1_h);
// speed is approximately 26.8224 m/s
// auto invalid = 1_yd + 1_m; // Explicit conversion required
// auto wrong = 1_mi + 1_s;   // Different dimensions: rejected
```

## Numeric operations

- `quantity<U>{}` initializes to zero; `quantity<U>{x}` explicitly supplies a raw value.
- `.value()` explicitly extracts that value in **U**, not necessarily in SI units.
- `unit_cast<To>(q)` is allowed only between the same dimension. It multiplies by
  a compile-time-known scale factor; it may do runtime arithmetic.
- `+`, `-`, `+=`, `-=` require `SameUnit`. The result uses the left operand's unit type.
- `*`, `/` between quantities add/subtract exponents and multiply/divide scale ratios.
- `q * scalar`, `scalar * q`, `q / scalar`, `q *= scalar`, `q /= scalar` preserve units.
- `scalar / q` inverts the unit. Unary `+` and `-` preserve it.
- `==`, `!=`, `<`, `<=`, `>`, `>=`, `<=>` require `SameUnit`. Ordering follows `double`
  partial ordering, including unordered NaN comparisons.
- `pow<N>(q)` accepts integral N from -8 to 8. Dimension exponents and ratio arithmetic
  must remain representable. `pow<0>(q)` is defined as dimensionless 1, including q=0.

There are no implicit quantity-to-double or cross-unit conversions. A dimensionless
result remains a quantity: use `unit_cast<one>` to normalize its scale, then `.value()`.
For example, `(1_km / 1_m).value()` is 1 in a unit whose scale is 1000;
`unit_cast<one>(1_km / 1_m).value()` is 1000.

## Optional output

Include `format.hpp` or the umbrella `dimenguard.hpp` for `operator<<`,
`unit_symbol<U>()`, and `dimension_string<D>()`. Known units receive familiar
symbols; custom derived units use an explicit ratio and SI factors.

## Optional educational diagnostics

```cpp
constexpr auto verdict = dimenguard::analyze<
    dimenguard::operation::add, dimenguard::metre, dimenguard::second>();
auto detail = dimenguard::explain<
    dimenguard::operation::divide, dimenguard::metre, dimenguard::second>();
```

`analyze` is a `consteval` type-compatibility check. The operations are `add`,
`subtract`, `multiply`, `divide`. Verdicts are `valid`, `dimension_mismatch`,
`explicit_conversion_required`.

`explain` returns strings describing both operands, the result dimension when
meaningful, a reason and a suggested action. It has runtime string/allocation costs.
It does not weaken the constraints on actual arithmetic operators. It explains
dimension/scale policy, not numerical correctness or limits of `std::ratio` arithmetic.

## Explicit limits

- This version supports three of the seven SI base dimensions, not all of SI.
- A scalar double representation only; normal rounding, overflow, underflow,
  NaN, infinity and division-by-zero behaviour are not prevented by dimensional typing.
  Invalid constant evaluation may be rejected by the compiler; dynamic arithmetic
  receives no extra runtime guard. The CLI separately rejects non-finite input/results.
- Dimension exponents are bounded; rational scale operations must fit `std::intmax_t`,
  including intermediate values required by `std::ratio`. Exceeding this fails compilation.
- No Celsius/Fahrenheit offsets, roots/fractional powers, vectors or angle semantics.
- Equal dimensions do not imply equal physical meaning (for example, energy vs torque).
  Quantity-kind safety is not implemented.
- A caller can still label externally supplied feet as metres incorrectly. Types
  cannot recover the meaning of mislabeled input, and `.value()` deliberately leaves
  the type-safe boundary.
- No cross-platform execution guarantee is inferred from a successful Linux build.
