# Build and test L-001

## L-002 extension

The same commands below build and test L-002 as well. Its public API is
`include/jacks/deposit.hpp`, implementation `src/deposit.cpp`, and tests
`tests/deposit_test.cpp`. Run configure after pulling the new targets. The helper
and build directory retain their existing names; there is no second setup to learn.
With the deposit scaffold, expect **13 existing passes and 1 deposit skip**.
Tommy owns replacing its throwing stub and skipped placeholder after review.
The L-001 initial-scaffold description below is historical; L-001 is now implemented.

Engineering owns the harness. Tommy owns the valuation test and implementation.
This scaffold awaits peer review; it supplies neither solution.

## Where work goes

```text
CMakeLists.txt                 build targets and pinned test dependency
include/jacks/valuation.hpp     shared API: types and function declaration
src/valuation.cpp              Tommy's implementation (currently throwing stub)
tests/valuation_test.cpp        Tommy's tests (currently GTEST_SKIP)
tests/infrastructure_test.cpp   engineering's non-quant smoke check
tools/l001.ps1                  Windows terminal helper
docs/build.md                  these instructions
learning/submissions/L-001/     explanation and red/green record
build/l001/                    generated files/dependencies; ignored by Git
```

Callers include the header without needing implementation details. Tests live
outside `src/` so GoogleTest stays test-only. Generated files go in `build/`
because they can be recreated and should not enter commits. The learning folder
points to this shared component; it does not hold duplicate code.

## Configure, build, test

At the repository root, paste this into Windows PowerShell or PowerShell 7.
It uses the already installed Codex runtime; no PowerShell install is required:

```powershell
$engPwsh = 'C:\Users\18589\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
& $engPwsh -NoProfile -File ./tools/l001.ps1 configure
& $engPwsh -NoProfile -File ./tools/l001.ps1 build
& $engPwsh -NoProfile -File ./tools/l001.ps1 test
```

Omit the final action word for all three. The helper requires PowerShell 7;
running it directly with Windows PowerShell 5.1 is unsupported. The explicit
runtime invocation above was verified from Windows PowerShell 5.1 on this machine.
Configure selects installed VS2019 x64
and downloads pinned GoogleTest the first time (internet access needed). Build
compiles and links Debug executables. CTest runs GoogleTest cases and reports
failures. The helper handles this machine's PATH issue without changing machine
settings. Fix/report a failed command before continuing. No global install needed.

**Initial result: one infrastructure pass, one component skip.** This is not
valuation correctness. CTest can print aggregate success with skipped tests;
check individual results. The stub deliberately throws `std::logic_error` on
every call. Final domain errors use the variant, not that temporary exception.

## Fixed interface and first TDD cycle

`jacks::calculate_equity(std::int64_t cash_minor_units, std::int64_t quantity_units,
std::optional<std::int64_t> mark_minor_units)` returns
`std::variant<jacks::Equity, jacks::ErrorCode>` (alias `ValuationResult`).
`Equity::minor_units` stores signed 64-bit money. Error names: `invalid_input`,
`missing_mark`, `overflow`. The header specifies units and validation precedence.
These types carry values; the function validates them.

Your first missing-mark example is accepted. Replace the skipped placeholder in
`tests/valuation_test.cpp` with your GoogleTest check. Justify its expectation
from the contract before coding. `ASSERT_*` stops the current test on failure;
`EXPECT_*` reports failure and continues. The infrastructure test illustrates
syntax with an unrelated string, not a valuation example.

1. **Red:** build/run and confirm the intended test fails. Compiler errors are
   not behavioral evidence; the unimplemented stub's exception is expected until
   you begin implementing behavior.
2. **Green:** make the smallest implementation change satisfying the checked contract.
3. **Refactor:** improve clarity and rerun tests.

Cover meaningful rejection and overflow boundaries with independently justified
expected values as work grows. Do not copy implementation calculations into
expected results. Engineering supports the harness; risk/research review behavior
and expectations. No further quiz or framework design is needed from you.

Dependency: official [GoogleTest v1.14.0](https://github.com/google/googletest/tree/f8d7d77c06936315286eb55f8de22cd23c188571),
pinned by full commit in CMake; GoogleMock and installation disabled. Its
[CMake instructions](https://github.com/google/googletest/blob/f8d7d77c06936315286eb55f8de22cd23c188571/googletest/README.md)
describe the integration. Only this Windows configuration was executed here.
Record your actual commands and results with your submission.
