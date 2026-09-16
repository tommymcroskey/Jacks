# L-002 work locations

Tommy owns `src/deposit.cpp` and `tests/deposit_test.cpp`. The public interface is
`include/jacks/deposit.hpp`; behavior is specified in
[L-002](../../assignments/L-002.md). Engineering supplied only a throwing stub and
explicitly skipped test placeholder. Do not count that skip as deposit correctness.

After header/harness review and the short first-case discussion, replace the
placeholder with your first test. Implement the transition through TDD, reusing
`calculate_equity`. Keep the L-001 tests passing. Record actual commands/results
and brief red/green observations here; no reconstruction of past history is needed.

Use the existing [build guide](../../../docs/build.md) commands. The helper keeps
its `l001.ps1` name and `build/l001` directory for continuity, but configures/builds/
tests the whole project, including deposit. Run configure once after pulling new
targets, then build and test after edits. Engineering owns build/setup problems.

Initial scaffold result: 13 existing passes and 1 deposit skip. The unimplemented
stub deliberately throws; final domain failures must use the agreed DepositError.
Keep any first expected-value example your own; none is implemented here.
