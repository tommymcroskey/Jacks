# RES-004: executable deposit replay acceptance scenarios

Author: Quant Researcher agent. Project: P001. Date: 2026-09-18.
Base: `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.
Status: draft executable scenarios; runtime verification blocked on the replay
implementation. Not ready for acceptance or integration as tested behavior.
Exact header/contract dependency:
`839df685f482a3597b3e8d83224c4e25f16a4d06` (`include/jacks/replay.hpp` and
`docs/deposit-replay.md`). Research reviewed that new-run trace contract and found
it consistent with the scenarios below; no implementation approval is implied.

## Independent economic expectations

These synthetic inputs extend the risk-reviewed RES-003 hand calculations into
checks of engineering's replay trace. They use no market data. Every expected
balance below is a literal, not calculated by an alternate ledger or profit
implementation. Money is integer minor CU (100 = 1 CU), holdings whole units,
and mark minor CU per unit. Opening equity is fixed externally; initial cumulative
deposits and accepted sequence are zero. Events remain in caller order.

| Scenario / accepted row | Cash | Quantity | Mark | Net deposits | Sequence | Equity | Expected profit |
| --- | ---: | ---: | --- | ---: | ---: | ---: | ---: |
| A opening | 10000 | 0 | absent | 0 | 0 | 10000 | 0 |
| A deposit 2500, sequence 1 | 12500 | 0 | absent | 2500 | 1 | 12500 | 0 |
| A deposit 750, sequence 3 | 13250 | 0 | absent | 3250 | 3 | 13250 | 0 |
| B opening | 10000 | 2 | 3000 | 0 | 0 | 16000 | 0 |
| B deposit 5000, sequence 1 | 15000 | 2 | 3000 | 5000 | 1 | 21000 | 0 |
| B deposit 2000, sequence 2 | 17000 | 2 | 3000 | 7000 | 2 | 23000 | 0 |

A: 2500 + 750 = 3250 of funding; 13250 - 10000 - 3250 = 0 profit.
B: holdings stay worth 2 x 3000 = 6000; 23000 - 16000 - 7000 = 0 profit.
Sequence 3 in A is an allowed gap, not an omitted cash movement. These examples
must preserve the distinction between absent mark and a present numeric mark.

Also check an empty stream against B's opening snapshot: no accepted events,
final equity 16000 and net deposits 0. Replay prefixes of A and B must match their
literal corresponding rows. This detects a changed opening baseline, dropped
funding, omitted holdings, or a trace containing only the terminal answer.

The checker compares actual opening/accepted/final trace fields with these
literal expectations. Profit zero is documented reviewer evidence only: no profit
function is supplied or called before Tommy's accepted L-003 artifact. Passing
trace checks must not be described as testing that future calculation.

## Boundary and ownership

Engineering owns replay/API/build wiring; research owns these expected histories
and their executable comparisons. Risk independently reviews arithmetic and supplies
adversarial/failure-prefix checks. Tommy owns L-003 implementation and unit tests.
No mark changes, withdrawals, trades, parsing, persistence, or performance claims.

## Executable handoff and current evidence

`tests/replay_economic_test.cpp` contains two GoogleTest cases. Each invokes
`replay_deposits` for empty, one-event and two-event input, compares every exposed
state/equity/index field with literals, expects no event failure, and checks input
snapshot/vector preservation. It never invokes a reconciliation/profit helper.
`tests/research/CMakeLists.txt` defines `jacks_replay_economic_tests`, linking
engineering's `jacks_replay` and existing GoogleTest. Engineering owns the root
`add_subdirectory(tests/research)` wiring inside `BUILD_TESTING`.

Author checks performed: six documented equity/profit rows independently reconciled
with direct PowerShell arithmetic; source inspected against the exact committed
header above; staged whitespace check. No compile, link, CTest or runtime success
is claimed. The dependency currently supplies only header/docs, not a runner.
No substitute runner or apprentice implementation was introduced to make tests pass.

After the reviewed runner/root wiring is available, configure and build the normal
project, then run `ctest --test-dir build/l001 -C Debug -R ReplayEconomic
--output-on-failure --no-tests=error` (one command). Record the exact runner commit,
commands, observed results, and a controlled wrong-expectation failure before
claiming executable acceptance. Also retain the existing regression suite.

Next owner: engineering supplies the committed runner/wiring; research resumes
runtime verification; risk independently reviews literals and executed evidence.
Coordinator owns PR transport. This draft checkpoint is not completion of runtime
acceptance; that explicit dependency is the blocker.
