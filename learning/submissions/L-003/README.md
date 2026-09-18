# L-003 submission work area

Tommy owns `src/profit.cpp` and `tests/profit_test.cpp` after the reviewed scaffold
handoff. The frozen interface is `include/jacks/profit.hpp`; see the
[assignment](../../assignments/L-003.md) and [build guide](../../../docs/build.md).

Engineering supplied only a throwing implementation placeholder and a skipped
test with a compile-time signature check. Neither establishes profit behavior.
Replace them with your implementation and focused assertions. Final domain errors
use `ProfitError`; the scaffold's exception is not the intended API behavior.

Use the existing configure/build/test helper. The separate `jacks_profit_tests`
target is included in full CTest runs; replay does not link to the pending profit
implementation. Keep the accepted valuation/deposit tests passing.

Add a brief explanation and actual commands/results as you work. Name the tested
commit, or say that a run included uncommitted edits. Distinguish failed, passed,
and skipped tests. No retrospective reconstruction or large log dump is required.

Report:

09/18/2026
Finished project scope. Implements profit function with input and boundary checks as stated in spec. 
Commands Run:
cmake --build build/l001 --config Debug
ctest --test-dir build/l001 -C Debug --output-on-failure
Notes:
Taking advantage of non negative input types allows boundary check to only acknowledge one side of the boundary. True INT64_MAX overflow is impossible with stated specs.
