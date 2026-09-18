# RES-004: executable deposit replay acceptance scenarios

Author: Quant Researcher agent. Project: P001. Date: 2026-09-18.
Base: `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.
Status: local runtime verification complete; awaiting independent executable review.
Exact header/contract dependency:
`839df685f482a3597b3e8d83224c4e25f16a4d06` (`include/jacks/replay.hpp` and
`docs/deposit-replay.md`). Research reviewed that new-run trace contract and found
it consistent with the scenarios below. Runtime dependency is engineering's
`976e5ec72647121a415026a5d6cdd3a55ce37282`; merged into this research branch as
`b27f08bae82fb9012e1fa1646f75872968738bf2` before the additive root wiring below.

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
engineering's `jacks_replay` and existing GoogleTest. With engineering's explicit
handoff, research added the single root `add_subdirectory(tests/research)` line
inside `BUILD_TESTING`; no replay or apprentice source was changed.

## Reproduction and observed results

From the repository root, using the installed PowerShell 7 runtime and existing
VS2019/MSVC 19.29 x64 Debug toolchain:

```powershell
$resPwsh = 'C:\Users\18589\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
& $resPwsh -NoProfile -File ./tools/l001.ps1 configure
& $resPwsh -NoProfile -File ./tools/l001.ps1 build
& $resPwsh -NoProfile -File ./tools/l001.ps1 test
```

Initial configure failed because the sandbox blocked the pinned GoogleTest clone.
The authorized network retry succeeded without changing the dependency pin or
machine settings. Configure, compile/link and the full CTest run then succeeded:
**41 executed passes, 1 explicit Profit.ApprenticeImplementationPending skip**.
That comprises 35 existing tests, 4 engineering replay tests, and 2 research tests
(six replay invocations). The skip is not profit correctness evidence.

Failure sensitivity: temporarily changed only scenario A's final expected equity
from 13250 to 13251 in the research source. Rebuilt and ran the following with the
installed CTest executable; one research case failed at accepted and final equity,
the other passed, and CTest returned **8**:

```text
ctest --test-dir build/l001 -C Debug -R ReplayEconomic --output-on-failure --no-tests=error
```

Restored 13250, rebuilt and reran the complete suite: again **41 passes, 1 skip**.
Git diff confirmed no residual scenario-source change from the draft. The probe
was never committed. Six documented economic rows also passed independent direct
PowerShell arithmetic; staged whitespace checks passed. The submitted code state
is the merged dependency plus root wiring; this report is the only further change.

These are local executable observations, not a hosted CI run, Release/platform
coverage, a complete risk suite, or acceptance of the pending profit function.
No substitute runner or profit implementation was used. Next owners: risk and
engineering reproduce/review the exact submitted head; coordinator handles PR
transport and dependency integration. No remaining local runtime blocker.
