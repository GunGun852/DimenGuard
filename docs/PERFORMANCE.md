# What "zero additional overhead" means here

The objective is parity with the **same arithmetic**, using the same compiler and
optimization flags, expressed directly in `double`. It does not mean no instructions,
no floating-point rounding, or that runtime inputs are evaluated at compile time.

For example, converting a runtime number of feet to metres requires scaling by
0.3048. A type-checked cast should do the same work as the corresponding handwritten
multiply. The conversion factor can be computed at compile time; the input value
does not have to be known then.

## Recorded experiment

Compiler: GCC 14.2.0, Linux x86-64. Flags:

```text
-std=c++20 -O2 -S -fno-ipa-icf
```

The final flag disables identical-code folding so each function keeps a separately
inspectable body. All six functions take **runtime parameters**. They do not use
all-constant examples or timing loops that the optimizer can discard.

| Typed function | Raw baseline | Observed normalized instruction text |
|---|---|---|
| `typed_speed` | `raw_speed` | Identical |
| `typed_force` | `raw_force` | Identical |
| `typed_feet_to_metres` | `raw_feet_to_metres` | Identical |

In this environment `sizeof(quantity<metre>) == sizeof(double) == 8`; the core tests
also verify alignment, standard layout and trivial copyability. A size assertion
alone would not prove execution-speed parity, so both layout and generated code
are examined.

Saved evidence: [result.json](../benchmarks/evidence/result.json) and
[overhead.s](../benchmarks/evidence/overhead.s).

## Reproduce

```sh
python3 tools/verify_overhead.py --compiler g++
```

The helper is deliberately limited to GCC's Linux assembly format. It writes fresh
evidence under `build/overhead/`, compares instruction text and exits nonzero if
the results differ. A difference needs investigation; it does not automatically
mean a practical slowdown. The helper is optional and not required to use the library.

For Clang, MSVC or another architecture, compile `benchmarks/overhead.cpp` with the
same optimization flags for both versions and inspect the resulting functions with
that compiler's assembly/disassembly tools. Do not label an untested configuration
as verified.

## What the experiment does not claim

- Universal parity for every expression, compiler, architecture or debug build.
- Zero compile-time cost; template checking can increase compilation work.
- Zero cost for streams, strings, CLI parsing or the optional explanation layer.
- That dimensional types prevent floating-point or physical-model errors.
- That this experiment is a benchmark against mp-units or Boost.Units.
