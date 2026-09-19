# Jacks status

Updated 2026-09-19. Exact PR evidence and current coordinator handoffs supersede
older snapshots. Specialists run bounded assignments, not continuous background work.

## Completed foundation

- Tommy: L-001 valuation (PR #12), L-002 deposits (PR #19), L-003 profit
  (PR #31), and L-004 mark updates (PR #37).
- L-004 integrated at `4a850fcfe0c984f2c343f8e2dba1558c405a86e8`.
  Engineering and Risk independently accepted source
  `cb2ad17c384b970168abf07a50d16db8396d1b3e`, each reproducing 79/79 tests,
  zero skips/failures, MSVC 19.29 x64 Debug. Hosted PR CI passed; coordinator
  configured/built/tested the actual merged commit with the same 79/79 result.
- Senior deposit replay (PR #26), research/risk checks (#25/#28), Windows CI
  (#27), account demo (#34), and CMake maintenance (#36) are integrated.
  The demo currently connects valuation/deposits/profit; trading integration
  is the next milestone, not a completed backtester.
- Review and postintegration evidence is attributed on each PR. Shared GitHub
  credentials do not create separate reviewer-account identities.

## Agreed direction

See [synthetic trading lifecycle](decisions/P001-synthetic-trading-lifecycle.md).
All three specialists agree: one explicit synthetic buy/sell account history,
then a historical study only after Research's data-semantics gate. Tommy
delegated this direction decision to the seniors.

One apprentice contribution is reserved: L-005 supplied-fill accounting and
focused tests. Engineering owns the reviewed exact header/assignment/scaffold.
The ready handoff and merged scaffold PR establish when implementation starts.
No additional apprentice assignment or senior replacement solution.

| Owner | Bounded responsibility |
| --- | --- |
| Tommy | Implement/test the one reviewed supplied-fill transition |
| Engineering | Interface/build setup, mentoring, later scripted composition/report |
| Research | Literal expected history, interpretation, historical-data requirements |
| Risk | Contract/failure review, independent reconciliation and timing checks |
| Team room | Scope/status, exact review handoffs and integration |

The senior composition waits for the accepted apprentice function. Research
and Risk can prepare independent expected results and checks concurrently.

## Working agreements

- Mark policy and data interpretation stay separate from accounting.
  A valuation mark is not an execution guarantee.
- Existing apprentice `src/` code is preserved; seniors may provide newly
  authorized scaffolds but do not take over his implementation/tests.
- Engineering owns small build/tooling/documentation maintenance after
  milestones. A weekly Friday 10 a.m. maintenance heartbeat is active in this
  team-room task. It routes useful bounded work to existing specialists and
  stays quiet without an actionable change. It is not a continuous company loop.
- Routine reviewed merges remain authorized. No force-push or direct-main edits.
- Four merged remote Research branches remain undeleted after automatic approval
  review rejected relayed deletion authority; a direct owner answer is pending.
  Do not retry through another agent or transport. Unmerged historical reviews
  remain preserved. This does not block the current milestone.

## Existing tasks

Host local; each specialist uses its own worktree/feature branch.

- Team room: `01a0a692-cdb4-7f20-9fde-95544eaed3a1`
- Engineering: `01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941`
- Research: `01a0a6b7-14f7-79e2-9dee-48bddb4a650f`
- Risk: `01a0a6b7-2083-7d30-aeda-b8e02f781461`

## Limits

The evidence is Windows Debug, not exhaustive arithmetic proof, Release or
sanitizer coverage, alpha, execution realism or trading readiness.
No purchases, broker access, raw Nasdaq pipeline, generic strategy/event
framework, borrowing, shorting, optimization or second apprentice task.
