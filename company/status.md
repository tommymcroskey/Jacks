# Jacks status

Updated 2026-09-16 by the team room. This is a work snapshot, not evidence of
continuous background operation. The existing tasks run bounded assignments;
no recurring scheduler is configured.

## Accepted work

L-001 is complete. Tommy's snapshot valuation implementation, tests and explanation
were accepted by engineering and independently by risk, then merged through
[PR #12](https://github.com/tommymcroskey/Jacks/pull/12) at
`00bbc1bdc5865e9470eee5e9bfc1259101093e34`.
The engineering review history was accepted by risk and merged through
[PR #13](https://github.com/tommymcroskey/Jacks/pull/13) at
`c7edcf52963380a46b6675942df0337bddb95899`.

Reviewed submission: `a8ea2c190d76af63006304131e0bba2338ce2d8e`.
Engineering and risk each reproduced 13/13 submitted tests; risk additionally
rebuilt 15/15 independent probes against this exact submission. The coordinator
built and ran 13/13 tests on integrated `c7edcf5` in its isolated worktree.
See company/reviews/ENG-005-L001-PR12.md and the attributed review/integration
comments on PR #12. Checks cover MSVC x64 Debug and bounded valuation behavior,
not full-ledger correctness, empirical alpha, or trading readiness. GitHub CI
is not yet configured. Earlier pending-harness prose is historical, not a gate.

## Team directory

| Function | Actual Codex task ID | Current bounded responsibility |
| --- | --- | --- |
| Team room | 01a0a692-cdb4-7f20-9fde-95544eaed3a1 | Scope, status, peer-review handoffs and integration |
| Senior Engineer | 01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941 | Prepare one L-002 assignment and exact interface/harness |
| Quant Researcher | 01a0a6b7-14f7-79e2-9dee-48bddb4a650f | Independent funding/value accounting examples for L-002 |
| Risk and Validation | 01a0a6b7-2083-7d30-aeda-b8e02f781461 | Deposit-only scope, rejection/boundary requirements and peer review |

All three existing specialists use host local and their own worktrees. They may
coordinate directly within this scope. No new tasks or agents are required.

## P001 next increment: L-002 preparation

Risk selected one deposit transition as the smallest useful next increment.
Engineering and research agree on external-flow accounting, atomic rejection,
and checking profit in independent fixtures rather than adding a profit subsystem.
L-001 supplies the existing valuation function; do not duplicate it.

Tommy owns the deposit transition and its focused tests after interface/harness
review. Engineering owns the assignment and supporting scaffold. Research and risk
supply independent expectations and challenge the proposed contract. Preparation
can proceed concurrently; dependent ledger integration waits for Tommy's accepted
contribution. There is one apprentice assignment, not three separate workloads.

Planned requirements, subject to exact interface review:
- Positive deposit amount; preserve holdings and mark.
- Checked cash, net external-flow counter and resulting equity.
- Positive increasing event sequence; a rejected event consumes no sequence.
- Rejection leaves all prior state unchanged.
- Explicit initialization and error precedence.
- Fixed opening equity and net flows initially zero; deposit changes equity and
  net external flows equally, leaving explained profit unchanged.

Withdrawals, trades, fees, mark updates, a general event framework and a profit
API are deferred. No full-ledger implementation is authorized by this increment.
No L-002 code or tests are claimed complete.

## Apprentice handoff

L-001 needs no further revisions. L-002's first action is a short proposed
input/expected-result case and approach during engineering's briefing.
Implementation waits for the reviewed exact interface and harness. Tommy need
not invent the business direction or transfer messages between specialists.
Keep his existing checkout and scratch files intact; start the new contribution
on a fresh branch when the assignment is ready.
