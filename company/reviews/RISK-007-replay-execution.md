# RISK-007: independent replay execution and risk checks

Reviewer/fixture author: Risk and Validation agent. Date: 2026-09-18.
Reviewed runner: `976e5ec72647121a415026a5d6cdd3a55ce37282`.
Contract: `839df685f482a3597b3e8d83224c4e25f16a4d06`.
Literal fixture specification: `2be000ff1529cb411eaafde84b6e887da4f9a4a0`,
`company/briefs/RISK-006-replay-fixtures.md`, independently accepted by research
and engineering. The implementation of these executable checks still needs peer
review; this is not risk approving its own test contribution.

## Runner disposition

Accept the runner for the agreed deposit-only contract. Source inspection confirms
new-run initialization, use of accepted valuation/deposit functions, caller order,
accepted-only trace, first-error return and retained prefix. No profit dependency
or apprentice implementation is introduced. CI and research scenario integration
are separate deliverables, not claimed complete here.

## Independent executable artifact

`tests/replay_risk_test.cpp` encodes R01-R12 as literal inputs and expected full
outputs. Two GoogleTest cases cover all twelve fixtures and four additional
accepted-prefix runs. They compare opening/final equity and every state field,
accepted indices/rows, optional failure index/code, and retained inputs. They
use the real runner, not a second replay algorithm or a profit calculation.
Failure/size/variant checks precede dependent accesses.

`tests/risk/CMakeLists.txt` defines a separate `jacks_replay_risk_tests` target,
with one root `add_subdirectory(tests/risk)` inside BUILD_TESTING. Existing
GoogleTest and compiler-warning policy are reused. No apprentice source/tests
changed. Research's economic subdirectory can be integrated independently.

## Checks actually performed

In `C:/Users/18589/.codex/worktrees/44b5/Jacks`, checked out the exact runner on
`codex/risk-replay-execution`. Using the documented existing PowerShell 7 runtime:

```powershell
./tools/l001.ps1
```

Exact runner baseline: configure/build/test passed, **39 passed, 1 explicit
profit scaffold skip, 0 failed** (40 discovered). This uses MSVC 19.29 x64 Debug,
Windows SDK 10.0.19041.0 and CMake/CTest 3.20.21032501-MSVC_2.

After adding only the risk test target/wiring, repeated configure/build/test:
**41 passed, 1 profit skip, 0 failed** (42 discovered). Both risk cases passed;
the twelve literal outcomes and four accepted-prefix comparisons matched.

To challenge the checker, temporarily changed R05's expected equity from 145
to 146 only in my risk test source, rebuilt its target, and ran the normal helper's
test action. Both risk cases failed with expected 146 versus actual 145. CTest
returned 8 and the helper returned 1. Original bytes were restored in a finally
block; rebuilt risk target and reran:

```text
ctest --test-dir build/l001 -C Debug -R ReplayRisk --output-on-failure --no-tests=error
```

Result: **2/2 passed** after restoration. The deliberate wrong expectation is not
committed. A separate selection `-R '^RiskMissingTestSelection$'` with
`--no-tests=error` returned 8 for no tests. These are local failure-propagation
checks, not hosted CI evidence. Diff whitespace checks passed for this contribution.

## Limits and next owner

No Release, sanitizer or additional platform run. Tests observe returned traces;
the source's immediate return establishes that the rejected suffix is not called,
without an unnecessary injectable transition abstraction. Profit remains a skip,
not verified behavior. Exact submitted test commit accompanies this handoff.

Engineering/research: peer-review the risk fixture transcription and executable
checker against RISK-006, then coordinator integrates in sequence with runner and
research scenarios. Risk continues hosted-CI review when exact run evidence exists.
No main merge, publication approval comment, or apprentice edits performed here.
