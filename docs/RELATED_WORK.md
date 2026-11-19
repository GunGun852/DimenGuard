# Motivation, related work and innovation claim

## Motivation

Unannotated numeric values hide units. A function taking two doubles cannot express
that the first is a length and the second is a duration. DimenGuard places units in
the type system and preserves dimension/scale information through arithmetic.
The optional explanation API makes those rules inspectable for students.

## Existing solutions

Official documentation reviewed on 16 November 2026:

- [mp-units: About](https://mpusz.github.io/mp-units/latest/getting_started/about/):
  describes compile-time dimensional analysis, C++20 concepts, readable diagnostics,
  value-preserving conversions, and richer quantity semantics.
- [Boost.Units: Quick Start](https://www.boost.org/latest/doc/html/boost_units/Quick_Start.html):
  describes general dimension algebra, explicit conversions and optimized runtime
  overhead relative to equivalent unchecked code.

| Capability | DimenGuard 1.0 | Existing-library context |
|---|---|---|
| Compile-time dimensions | Length/mass/time subset | Already established |
| C++20 constraints | Named concepts on arithmetic/conversion | Already used by mp-units |
| Units and conversions | Positive rational scales; explicit casts; double only | Existing libraries offer broader support |
| Zero additional core overhead | Evidence for three GCC examples | An established goal, not a new invention |
| Educational explanation API | Structured operation verdict + exponent vectors + suggestion | A concrete feature to evaluate; no exclusivity claim established |

This comparison is based on documentation, not a comprehensive implementation audit
or performance comparison. We have not established that existing libraries cannot
implement or replace any of the proposed features. Smaller code, use of concepts,
and a different project name are not sufficient originality evidence.

## Defensible description of the delivered work

"DimenGuard is an educational C++20 units library with explicit conversion rules
and a reusable operation-explanation API. It shows how compile-time dimensional
constraints can reject invalid formulas and exposes the associated exponent
arithmetic to a teaching application. Its core behaviour is validated with positive
and negative compilation tests and limited generated-code comparisons."

This describes what is implemented without claiming a new dimensional-analysis
technique or guaranteed rubric marks.

## Work still needed for the coursework innovation criterion

Choose an actual use case, such as a formula-teaching tool that consumes structured
diagnostic records. Compare the closest existing approach using the same formulas
and record what extra code/integration is required. If no meaningful, difficult-to-
replace distinction emerges, state that honestly rather than inventing one.

Potential evaluation questions:

1. Can an application request an explanation of a hypothetical invalid operation
   without trying to compile an invalid expression?
2. Can it show both operands' exponent vectors and distinguish a wrong dimension
   from a correct dimension requiring an explicit scale conversion?
3. How much integration code is needed to offer that exact student-facing workflow
   with each compared library?

These are proposed comparisons, not results already obtained. Do not claim that
clearer errors alone are unique: mp-units explicitly targets error readability.
