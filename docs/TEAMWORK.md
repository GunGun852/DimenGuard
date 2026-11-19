# Suggested five-person work plan

This is a proposed ownership plan, **not a claim that five people have already
written the delivered code**. Replace placeholders with actual names and Git
accounts. Every member should review, explain, test and improve their assigned area.

| Owner | Proposed files/area | A concrete contribution to verify | Git account |
|---|---|---|---|
| Member 1 | `dimension.hpp`, architecture | Explain exponent arithmetic; extend boundary/type tests | To fill |
| Member 2 | `unit.hpp`, conversion code, custom-unit example | Verify rational scales and add a justified custom-unit example | To fill |
| Member 3 | `quantity.hpp`, `si.hpp` | Review constraints, implement/test an agreed arithmetic improvement | To fill |
| Member 4 | `tests/`, `benchmarks/`, verification helper | Run another compiler, record real failures/results, strengthen tests | To fill |
| Member 5 | `app/`, `diagnostics.hpp`, CMake integration | Improve explanation output or CLI usability and test it | To fill |

Agree on the public API together before parallel changes. Keep changes to shared
headers small and review each other's work. Each member documents the modules they
actually changed; documentation should not be assigned as the sole contribution
of a person who is expected to demonstrate programming work.

Use normal branches, commits and review. Do not fabricate historical commits,
backdate activity or assign someone else's code to a teammate. The AI-generated
starting point can be recorded as one honest baseline; subsequent team work should
reflect what actually happened.

## Suggested remaining sequence

1. Build on each available computer, read the examples, agree on interfaces and roles.
2. Work on the assigned improvements with small real commits and review.
3. Run the full suite, test the installed library in another program, record environments.
4. Complete the related-work comparison and accurately describe any distinction.
5. Freeze the demonstration, check the public repo/docs links, rehearse under one minute.

The coursework screenshot gives 24 November 2026 as the deadline. Plan from the
actual remaining time; the original "two-week" concept is not a new deadline.
