# DimenGuard Project Submission

**Course:** CS2310 Agentic Coding  
**Prepared:** 23 November 2026  
**Reviewed main revision:** [6861b85](https://github.com/GunGun852/DimenGuard/commit/6861b8547872cef09e20218ad9cdb49bf3faab3b)

DimenGuard is a header-only C++20 library that gives physical quantities strong types. It derives dimensions during arithmetic and rejects incompatible operations at compile time. Its console application demonstrates conversion across 19 unit symbols, full unit names, physics examples and a ten-entry session history.

## 1 Repository and documentation

- **Public repository:** https://github.com/GunGun852/DimenGuard
- **README and setup guide:** https://github.com/GunGun852/DimenGuard/blob/main/README.md
- **Documentation folder:** https://github.com/GunGun852/DimenGuard/tree/main/docs
- **API reference:** https://github.com/GunGun852/DimenGuard/blob/main/docs/API.md
- **Traditional Chinese guide:** https://github.com/GunGun852/DimenGuard/blob/main/START_HERE_ZH.md

## 2 Division of work

| GitHub account | Division of work | Commit evidence |
|---|---|---|
| [GunGun852](https://github.com/GunGun852) | Integrated the initial AI-assisted library, examples, tests and documentation; configured CI and corrected the compile-fail rebuild harness. | [Initial integration](https://github.com/GunGun852/DimenGuard/commit/6420c366fe0322b844b1e2a85adb07ebe1f630ff)<br>[CI setup](https://github.com/GunGun852/DimenGuard/commit/e970d4c4885c8d184f5be56f0c21240fec6e5124)<br>[Test harness fix](https://github.com/GunGun852/DimenGuard/commit/ebb9227eea5328c00f4cbf240d97387df49b2deb) |
| [wave2349](https://github.com/wave2349) | Integrated the interactive conversion menu, the last 10 successful conversions per session, history clearing, the launcher, and CLI/history tests. | [Interactive menu](https://github.com/GunGun852/DimenGuard/commit/a1145ae7c3faead93a2ab62881684d9a574d5eec)<br>[History integration](https://github.com/GunGun852/DimenGuard/commit/2c5902f1f8bf399848508ce2ffa8bce4a75c9b22) |
| [kamcyu4](https://github.com/kamcyu4) | Added yard and mile units, literals and formatting; extended conversion tests and updated the related documentation. | [Yard and mile](https://github.com/GunGun852/DimenGuard/commit/4b3a6d133a42a4f34cb0280f1f66d190c7eb1b08) |
| [kwcheng222-gif](https://github.com/kwcheng222-gif) | Added the unit full-name mapping and update patch, including labels such as s (second), yd (yard) and mi (mile). | [Unit name mapping](https://github.com/GunGun852/DimenGuard/commit/a7b49b224a6fcb1809dd614323cd48f48eb691f5) |
| [Mouse7701](https://github.com/Mouse7701) | Revised the Traditional Chinese setup and usage guide for clarity and consistency, and removed redundant documentation. | [Initial guide revision](https://github.com/GunGun852/DimenGuard/commit/54fd9f9c20e8849645718cbe180b0d883b64888e)<br>[Guide refinement](https://github.com/GunGun852/DimenGuard/commit/8dbbcbce02bfd0d184610e8023b4edf2279a8603) |

The linked commits record the submitted changes under each GitHub account. 
[Full commit history](https://github.com/GunGun852/DimenGuard/commits/main/) and [merged pull requests](https://github.com/GunGun852/DimenGuard/pulls?q=is%3Apr+is%3Amerged) provide additional collaboration evidence.

## 3 Reusable feature and justification

Existing libraries such as mp-units and Boost.Units already provide compile-time dimensional analysis and unit conversion. DimenGuard does not claim to invent these capabilities. Its reusable contribution is an educational explanation API for inspecting an operation without compiling the invalid expression itself.

The analyze API classifies an operation at compile time. The explain API returns a structured verdict, operand dimensions, a result dimension where applicable, an explanation and a suggested action. A teaching tool can reuse these results independently of the console menu. For example, adding metres to seconds identifies a dimension mismatch; adding metres to feet requests an explicit conversion; dividing metres by seconds explains how velocity is derived.

The meaningful problem addressed is explaining why a formula is accepted or rejected, rather than only accepting or rejecting it. Reproducing this interface with another units library would require an adapter for the same verdicts, explanations and explicit-conversion policy. This is a limited educational integration contribution, not evidence that other libraries cannot implement equivalent functionality. The optional explanation layer creates runtime strings; the typed arithmetic is separate.

**Implementation:** [Explanation API source](https://github.com/GunGun852/DimenGuard/blob/main/include/dimenguard/diagnostics.hpp); [API reference](https://github.com/GunGun852/DimenGuard/blob/main/docs/API.md).

**Comparison references:** [mp-units official overview](https://mpusz.github.io/mp-units/latest/getting_started/about/); [Boost.Units official quick start](https://www.boost.org/latest/doc/html/boost_units/Quick_Start.html).

## 4 Supplementary evidence

- **Automated validation:** 17 CTest entries passed on the reviewed revision. Coverage includes 10,091 core runtime checks, 11 compile-rejection cases with valid controls, 37 CLI cases and 22 interactive sessions. [Testing record](https://github.com/GunGun852/DimenGuard/blob/main/docs/TESTING.md); [Compile-fail sources](https://github.com/GunGun852/DimenGuard/tree/main/tests/compile_fail).
- **Cross-platform CI:** The successful run for revision 6861b85 checks Ubuntu, macOS and Windows, plus a separate assembly job. [Successful CI run](https://github.com/GunGun852/DimenGuard/actions/runs/44882685608); [Workflow](https://github.com/GunGun852/DimenGuard/blob/main/.github/workflows/ci.yml).
- **Performance evidence:** Three typed functions match normalized baseline assembly under GCC 14.2.0 with optimization. This supports the tested cases, not a universal zero-overhead claim. [Method and limits](https://github.com/GunGun852/DimenGuard/blob/main/docs/PERFORMANCE.md); [Recorded results](https://github.com/GunGun852/DimenGuard/blob/main/benchmarks/evidence/result.json).
- **Reuse and demonstration:** The custom-unit example extends the library without editing its headers. The one-minute demonstration script covers typed arithmetic, rejection, conversion and explanations. [Custom-unit example](https://github.com/GunGun852/DimenGuard/blob/main/examples/custom_unit.cpp); [Demo script](https://github.com/GunGun852/DimenGuard/blob/main/docs/DEMO_SCRIPT.md).

**Scope and limitations:** The library covers length, mass and time with derived dimensions and double-valued quantities. Dimensional safety does not guarantee physical correctness or numerical accuracy. Extreme-magnitude powers and subnormal CLI input remain numerical limitations.

**AI assistance:** AI-assisted development was used for the initial code, tests, demonstrations and documentation. The integration and subsequent revisions attributed above are recorded in the public Git history.

**Quick launch on macOS or Codespaces:** From the repository folder, run `bash run.sh`. The launcher builds the current source and opens the interactive menu. A C++20 compiler is required; full build and testing instructions are in the README.
