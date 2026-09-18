# P001: deposit replay and reconciliation infrastructure

Date: 2026-09-18. Coordinator record of actual engineering, research and risk
coordination. Base: `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.

## Accepted foundation

L-001 valuation and L-002 single-deposit transition are accepted. PR #19 merged
at the base above after engineering and risk each reproduced 35/35 submitted
tests with no skips. Coordinator configured/built/tested the integrated commit:
35/35 pass, clean worktree. Nonblocking source/test whitespace remains.
These Windows Debug checks establish bounded accounting evidence, not alpha.

## Next milestone

Build a deterministic deposit-only replay executable and independent acceptance
checks, run through actual automated CI. Replay means applying supplied events
one at a time and retaining their results. Reconciliation compares the recorded
balances with independently derived expected values.

All three specialists confirmed this final scope after considering alternatives.
Mark updates, withdrawals, trades, fees, parsing, persistence, strategy research
and a general event framework are deferred. Do not implement these as speculative
prerequisites.

Replay consumes caller order, never sorts, stops at the first rejected event,
retains the accepted prefix and final accepted state, and reports the failing
index and typed error. It does not process the suffix or mutate caller inputs.
Exact trace/API details receive peer review before dependent implementation.
Use accepted apply_deposit/calculate_equity rather than duplicating them.

## Real ownership and parallel work

| Owner | Bounded deliverable | What it waits for |
| --- | --- | --- |
| Engineering | Replay/trace API, runner, build wiring, actual CI, integration; one L-003 contract/header/harness | Peer agreement on exact interfaces; runner/CI do not wait for apprentice |
| Research | Literal economic trace fixtures and executable actual-versus-expected comparisons | Agreed trace API; no alternate transition/profit implementation |
| Risk | Independent invalid-event, overflow, prefix and rejection fixtures; interface and CI failure-path review | Exact artifacts to reproduce; no apprentice implementation dependency for deposit scenarios |
| Tommy | L-003 pure checked signed profit/reconciliation function and focused tests | Reviewed exact assignment/header/harness and brief approach discussion |
| Team room | Canonical status/scope, exact review handoffs, PR transport and integration | Required evidence and independent review |

Senior replay/CI and deposit-only acceptance scenarios can ship before L-003.
Only integration of calculated profit into reports waits for Tommy's accepted
artifact. The team must not write a substitute apprentice implementation.

## Apprentice boundary and business purpose

Proposed inputs are nonnegative signed-int64 opening equity, current equity and
cumulative net deposits, all in minor currency units. The mathematical result is
current equity minus opening equity minus net deposits; profit is signed.
The exact invalid-input, overflow and result contract must be reviewed before
coding. Correctness concerns the representability of the mathematical result;
intermediate arithmetic must not overflow.

Deposit-only scenarios with fixed holdings/mark have unchanged investment profit.
Negative/positive arithmetic examples for this helper do not imply that such
profit arose in a deposit-only run. This small function is reserved for Tommy.
No additional apprentice assignment is active.

First action: propose one opening-equity/current-equity/net-deposits example,
its expected signed profit and a sentence explaining the intended test.
Engineering supplies the final interface and scaffolding; implementation follows
their review, not a new architecture exercise.

## Milestone acceptance

- Existing 35 tests remain passing.
- Cash-only and fixed-holding multi-deposit, empty-stream and sequence-gap cases
  match literal independently calculated traces and funding totals.
- Rejected input preserves the accepted prefix and exposes the correct failing
  index/error; no later event is processed. Prefix results remain stable when a
  suffix is appended.
- CI actually configures, builds and runs checks; deliberate test failure and
  empty-test selection must fail meaningfully. Do not count skips as coverage.
- Record exact commits, commands, outputs, toolchain and limitations; peer-review
  each small PR and verify integrated results.
- CI configuration alone is not a successful CI run. No purchases, billing
  changes or secrets are authorized. Report unavailable runner access explicitly.

Authorization is for these bounded existing-specialist deliverables. No extra
agents/tasks, recurring scheduler, broker access or continuous operation is added.
