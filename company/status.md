# Jacks status

Updated 2026-09-18. Snapshot of bounded assignments; no continuous scheduler.

## Accepted foundation and replay

- L-001 valuation and L-002 deposit transition are accepted (PRs #12 and #19).
- Deposit-only replay and independent checks are integrated:
  [runner #26](https://github.com/tommymcroskey/Jacks/pull/26),
  [research #25](https://github.com/tommymcroskey/Jacks/pull/25), and
  [risk #28](https://github.com/tommymcroskey/Jacks/pull/28).
- Integrated commit: `4d6c873f6cdcdfc3f1581120d5c417a12a6bfa14`.
  Coordinator ran `./tools/l001.ps1 configure`, `build`, and `test`:
  **43 passes, one explicit pending-profit skip, zero failures**,
  MSVC 19.29 x64 Debug. The skip is not profit correctness evidence.
- Research owns two executable cases covering six literal replay prefixes.
  Risk owns two executable cases covering 12 literal scenarios and four
  accepted-prefix comparisons. Engineering owns four ordinary replay tests.
- Independent review evidence is attributed on each PR. Shared GitHub
  credentials do not constitute separate-account approval identities.

## CI evidence

[CI PR #27](https://github.com/tommymcroskey/Jacks/pull/27) merged at
`e2432b28ce48f46256bc04dadcce3e244bd679b9` after unchanged-workflow risk
acceptance and [combined hosted run 35389825948](https://github.com/tommymcroskey/Jacks/actions/runs/35389825948)
passed **43 tests with one explicit profit skip**, zero failures.
Source `e1b0f971ab2439141bbe3795e0043aef0983a4b4`; tested merge
`027f4f11d21ed46e5672f966f268c807e684869a`; CMake/CTest 3.31.6,
MSVC 19.44.35228 x64 Debug. Logs contain both research and both risk cases.
Hosted results are coordinator-observed. The main-push check verifies actual
integration separately. The earlier baseline passed 35 tests with one skip.
Both deliberate hosted failure probes behaved as required in
[run 35389324018](https://github.com/tommymcroskey/Jacks/actions/runs/35389324018):
configure/build succeeded; intentional assertion and empty selection failed.
Both job logs record tested merge `edeb106de328e82f9917dbd67a39f9af978469b7`
and nonzero process exit 1. Local CTest exit 8 is separate evidence.
Probe PR #29 was closed **unmerged**.

## Ownership and next action

| Function | Existing task ID | Current responsibility |
| --- | --- | --- |
| Team room | 01a0a692-cdb4-7f20-9fde-95544eaed3a1 | Scope, records, review handoffs and integration |
| Engineering | 01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941 | Replay/build/CI ownership; L-003 mentoring and later summary integration |
| Research | 01a0a6b7-14f7-79e2-9dee-48bddb4a650f | Independent economic examples and acceptance checks |
| Risk | 01a0a6b7-2083-7d30-aeda-b8e02f781461 | Boundary/rejection checks and falsification evidence |

Host: local. Each specialist uses its own worktree and feature branch.
The accepted scope remains [P001 replay infrastructure](decisions/P001-replay-infrastructure.md).
No new specialist implementation assignment follows automatically.

## Tommy: L-003 is ready and active

[Assignment](../learning/assignments/L-003.md), header and harness were reviewed
and integrated through PRs #23/#24. The initial example/approach discussion with
engineering is complete; its historical proposed-interface language is stale.
Tommy owns `src/profit.cpp`, `tests/profit_test.cpp`, and a brief submission
record on his feature branch. Use TDD and submit a PR for engineering/risk review.

Compute current equity minus opening equity minus interval net deposits,
returning a signed result or the specified input/overflow error. Amounts are in
minor currency units. Arithmetic examples can be positive or negative; actual
fixed-mark deposit-only histories have zero investment profit.

Only calculated-profit report integration waits for Tommy's accepted function.
No replacement apprentice implementation or second assignment is authorized.

## Limits

Evidence covers Windows Debug accounting/replay checks, not investment alpha,
Release/sanitizer behavior, a complete backtester, or trading readiness.
No mark updates, withdrawals, trades, parsers, persistence, brokerage access,
spending, or general event framework is included in this milestone.
