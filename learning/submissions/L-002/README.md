# L-002 submission: one deposit transition

Implementation and tests: Tommy. This submission record was reconstructed by
the team-room coordinator at Tommy's request on 2026-09-18.

## Scope and current result

`apply_deposit` validates an account snapshot and one deposit, then returns a
new state and equity or a tagged `DepositError`. It reuses `calculate_equity`.
Money is in integer minor units; quantity is in whole asset units. Deposits add
equally to cash and recorded net deposits, so owner funding is not investment
profit. Inputs are retained; the operation has no stored internal account state.

Engineering supplied the header, build wiring, and initial throwing/skipped
scaffold. Tommy owns the transition implementation and component tests.
See [assignment](../../assignments/L-002.md),
[build guide](../../../docs/build.md), and
[PR #19](https://github.com/tommymcroskey/Jacks/pull/19).

Latest code reviewed for this record:
`a9e1164107f0941e0dbf2136400d816cccf83308`.
Retrospective configure/build/test verification: **34 passed, 0 failed, 0 skipped**
(21 deposit tests plus 13 existing valuation/infrastructure tests).
This is a test result, not acceptance of all outstanding review findings.

## Commit-by-commit evidence

Every row below was built and, when compilation succeeded, tested again on
2026-09-18 from the exact committed source. These are **retrospective replay
results**, not a reconstruction of what Tommy ran before each commit.
Pass/fail/skip counts include the 13 existing tests.

| Commit | Change established by Git diff | Replayed result (pass / fail / skip) |
| --- | --- | --- |
| [fd3404b](https://github.com/tommymcroskey/Jacks/commit/fd3404bd28aaf7c42fe6edf63cbcfa0b01a229b4) | Adds deposit cases against the throwing stub. | 13 / 18 / 1; build succeeds, CTest exit 8. |
| [01c5144](https://github.com/tommymcroskey/Jacks/commit/01c5144d162423ae08de62432c4a5819dbadca7c) | Implements transition and checks successful cash/equity; net-deposits addition still unchecked. | 18 / 0 / 14; build/test exit 0. Only five deposit cases pass assertions. |
| [471b4a9](https://github.com/tommymcroskey/Jacks/commit/471b4a9bfaba09034bd461978225df90f1c9b884) | Checks net-deposits overflow; replaces remaining skips with assertions. | 32 / 0 / 0. |
| [bb771d6](https://github.com/tommymcroskey/Jacks/commit/bb771d68af46148b0bd3e07d90746c78122f8f1e) | Adds remaining returned-state checks to the normal success case. | 32 / 0 / 0. |
| [7a091bb](https://github.com/tommymcroskey/Jacks/commit/7a091bbb5f0911c33bb2cdbe2bf5a7c13cbefd0f) | Renames local newState to new_state; no behavior change. | 32 / 0 / 0. |
| [e6fbf3d](https://github.com/tommymcroskey/Jacks/commit/e6fbf3d3f896ea93b10c587b63bbc344297c2a59) | Adds Tommy's original red/green notes; no code/test change. | 32 / 0 / 0. |
| [85e041f](https://github.com/tommymcroskey/Jacks/commit/85e041f2c62b43196846dc1b3d08e1324caf1747) | Asserts optional mark presence before dereferencing. | 32 / 0 / 0. |
| [098b0f1](https://github.com/tommymcroskey/Jacks/commit/098b0f1612b7c50ed4a493b304b8c850e206f581) | Uses INT64_MAX in the competing sequence/overflow case. | 32 / 0 / 0. |
| [cb783c8](https://github.com/tommymcroskey/Jacks/commit/cb783c8597bee75bb02c8dac6e2c320a12abb6fa) | Isolates net-deposits overflow while cash/equity remain representable. | 32 / 0 / 0. |
| [53d929a](https://github.com/tommymcroskey/Jacks/commit/53d929a41bb009d28fd320e3284899469f97c662) | Adds input-preservation and cash-only maximum-deposit tests, with compile errors. | Build exit 1; tests not run. |
| [a9e1164](https://github.com/tommymcroskey/Jacks/commit/a9e1164107f0941e0dbf2136400d816cccf83308) | Uses field comparisons, calls apply_deposit, and checks DepositApplied. | 34 / 0 / 0. |

### Interpreting the history

- At `fd3404b`, the throwing scaffold causes 18 failures before later assertions
  or skips can be reached. That red run establishes missing implementation, not
  the strength of every intended behavioral assertion.
- At `01c5144`, stopping the stub's exceptions turns many cases into skips.
  The green CTest exit therefore does not mean all deposit cases were checked.
  Engineering separately reproduced unchecked net-deposits signed overflow in
  [ENG-007](https://github.com/tommymcroskey/Jacks/blob/fba47ecfdd5d2a3022be585de27c0542c994017e/company/reviews/ENG-007-L002-01c5144.md).
  That independent probe was not rerun as part of this documentation task.
- `471b4a9` fixes the unchecked addition and makes the suite fully asserting,
  but later review still identified weak fixtures and missing coverage.
  Passing tests are evidence about the assertions that exist.
- `53d929a` fails compilation: the boundary test calls the wrong function and
  checks the variant container rather than its success alternative; whole-struct
  equality also lacks an operator in these C++17 aggregates. The next commit
  corrects these issues. No stale executable was tested after a failed build.
- Tommy's original notes remain preserved at `e6fbf3d`. Their "(edited)" results
  describe author-reported working-tree observations, which cannot be identified
  as exact committed snapshots. This replay does not disprove those observations
  or claim to reproduce them. In particular, exact `01c5144`'s normal success
  test passes; the original note reports an edited version failing.

## Reproduction

From a configured developer terminal in a checkout of the desired commit:

```powershell
cmake --build build/l001 --config Debug
# Continue only if the build succeeded.
ctest --test-dir build/l001 -C Debug --output-on-failure --no-tests=error
```

On a fresh checkout, configure first. The verified local PowerShell 7 helper
handles the installed Visual Studio tools and child PATH normalization:

```powershell
$engPwsh = 'C:\Users\18589\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe'
& $engPwsh -NoProfile -File ./tools/l001.ps1 configure
& $engPwsh -NoProfile -File ./tools/l001.ps1 build
# Continue only if the build succeeded.
& $engPwsh -NoProfile -File ./tools/l001.ps1 test
```

The replay used Git archives in an isolated, ignored coordinator directory.
Only three tracked paths differ across these revisions (deposit source, deposit
tests and this README); archives were overlaid in commit order. Source/test
timestamps were refreshed to force recompilation, with one incremental build
directory and the same pinned GoogleTest source. Configure used VS2019 x64 and
`FETCHCONTENT_SOURCE_DIR_GOOGLETEST` pointing to the existing dependency.
Each successful build was followed by CTest with `--no-tests=error`; failed builds
were recorded without running tests. Counts came from individual CTest rows,
including skips, not its aggregate "100%" summary.

Replay environment: Windows, MSVC 19.29.30138.0, x64 Debug, CMake/CTest
3.20.21032501-MSVC_2, GoogleTest
`f8d7d77c06936315286eb55f8de22cd23c188571`.
Local raw logs, replay script and results.json are retained under
`C:/Users/18589/.codex/worktrees/jacks-team-room/Jacks/build/l002-history/`;
they are ignored local evidence, not a portable repository dependency.

## Review status and limits

The latest tests now compile and execute. The code at `a9e1164` still only checks
returned cash in the exact-limit case, and adds input-preservation assertions
only for success. Prior requests for boundary equity/net-deposits assertions and
representative rejection input-preservation checks are not claimed resolved here.
Existing source/test whitespace and documentation cleanup are also not changed
by this README-only task. Exact current acceptance belongs to the PR reviewers.

A separate retry test and defensive branches for unreachable candidate
missing-mark/invalid-input errors were explicitly withdrawn as acceptance gates;
see [review correction](https://github.com/tommymcroskey/Jacks/pull/19#issuecomment-5734509011).
The initial-state valuation still maps missing/invalid marks to invalid_state.

No Release, sanitizer, other-platform, exhaustive validation, ledger integration,
or strategy-performance result is claimed. This document reports committed
behavior and observed checks; it does not assign a proficiency score or invent
historical test execution.
