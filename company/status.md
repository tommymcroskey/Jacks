# Jacks status

Updated 2026-09-18. This is a snapshot; tasks run bounded assignments, not a
continuous background company. No recurring scheduler is configured.

## Completed

- L-001 snapshot valuation: accepted and integrated through PR #12.
- L-002 deposit transition: accepted by engineering and risk, merged through
  [PR #19](https://github.com/tommymcroskey/Jacks/pull/19) at
  `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.
- Both specialists ran 35/35 submitted tests with no skips on reviewed
  `6ce4dd444d9296ba467d3b812d9950aca686f3a8`. Coordinator verified integrated
  `8869447`: configure/build/test, 35/35 pass, clean worktree.
- README history maintenance was explicitly transferred to coordinator, reviewed
  and integrated through PR #20 into the submission before PR #19 merged.
- Nonblocking source/test whitespace remains. Evidence is MSVC x64 Debug;
  no Release/sanitizer/exhaustive or trading-readiness claim.
- Earlier pending-harness/assignment language is historical, not a current gate.

## Current milestone and team

See [accepted direction](decisions/P001-replay-infrastructure.md).
Final agreed scope is deposit-only ordered replay, executable reconciliation
checks and actual CI. Engineering, research and risk are authorized to progress
their independent bounded work; no implementation is claimed complete yet.

| Function | Existing task ID | Ownership |
| --- | --- | --- |
| Team room | 01a0a692-cdb4-7f20-9fde-95544eaed3a1 | Scope/status, review handoffs and integration |
| Engineering | 01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941 | Replay runner/trace, build/CI, L-003 interface/harness and mentoring |
| Research | 01a0a6b7-14f7-79e2-9dee-48bddb4a650f | Literal economic scenarios and executable comparisons |
| Risk | 01a0a6b7-2083-7d30-aeda-b8e02f781461 | Invalid-event/overflow/prefix checks, peer review and CI failure evidence |

Host: local. Each specialist uses its own worktree/branch. Only these three
specialists are used. Their next handoffs cite exact commits and actual results.

## Tommy: one next contribution

L-003 is a pure checked profit/reconciliation function: current equity minus
fixed opening equity minus cumulative net deposits. The result can be negative.
Engineering is preparing its exact contract/header/harness for peer review.
Tommy owns the implementation/tests after release, with one brief first-case
discussion. No new code is requested before the interface is ready.

Replay/CI and independent deposit-only checks can ship without L-003. Only
computed-profit report integration waits for Tommy's accepted function.
No mark-update, withdrawal, trade, parser, persistence or strategy work is included.
CI is currently not configured; a workflow file alone will not count as verified CI.
