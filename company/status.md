# Jacks status

Updated 2026-09-18. Bounded assignments, not a continuously running service.

## Completed

- L-001 valuation (PR #12), L-002 deposits (PR #19), ordered deposit replay
  (PR #26), independent research/risk checks (PRs #25/#28), and Windows CI
  (PR #27) are integrated.
- Tommy's L-003 checked profit implementation/tests shipped in
  [PR #31](https://github.com/tommymcroskey/Jacks/pull/31) at
  `65a2421f838702b7b328ca067609ff78e989f239`.
  Engineering and risk independently accepted exact source
  `ae93ef009200b231f64e4919dc594d80829c98a4`, each with 54 passes and zero skips.
  Coordinator also configured/built/tested the merged commit: 54/54 pass,
  zero skips/failures, MSVC 19.29 x64 Debug.
- Hosted PR [run 35404333702](https://github.com/tommymcroskey/Jacks/actions/runs/35404333702)
  passed 54/54 on tested merge `4dabc380001ba401f80f564d07f4c76627e82c52`;
  actual-main push [run 35404479818](https://github.com/tommymcroskey/Jacks/actions/runs/35404479818)
  also succeeded. Review details and probe limitations are attributed on PR #31.
- Submission README [PR #32](https://github.com/tommymcroskey/Jacks/pull/32)
  merged at `80639e57545541f2b1e4d22ceba5f9fdba36f46a` after engineering
  acceptance and hosted 54/54. README whitespace/wording nits were nonblocking;
  no extra apprentice revision is requested.
- CI failure propagation was verified with deliberate assertion/empty-selection
  probes, [run 35389324018](https://github.com/tommymcroskey/Jacks/actions/runs/35389324018).
  Temporary PR #29 was closed unmerged. Shared credentials do not constitute
  separate GitHub-account approvals.

## Current bounded work

Tommy asked to see the team's use of his work and to receive his next project.

| Owner | Deliverable |
| --- | --- |
| Engineering | Small terminal demonstration reusing valuation, deposits, replay and L-003 profit; separate exact L-004 contract/header/harness |
| Research | Independent literal economic expectations for demonstration and mark updates; no alternate implementation |
| Risk | Demo rejection/prefix review, mark-update domain and acceptance review |
| Tommy | Next reserved contribution: L-004 pure mark-update transition and focused tests, after reviewed interface handoff |
| Team room | Exact review/PR integration, canonical scope/status and completion reports |

Demo acceptance: completed fixed-holding deposits show opening equity16000,
ending23000, interval deposits7000, profit0, all in minor CU.
A duplicate event at index2 in four supplied events stops after two accepts.
Label it INCOMPLETE and label profit as accepted-prefix profit; never imply the
rejected suffix was processed. Engineering owns the executable/integration
checks. This is a demonstration of accounting infrastructure, not alpha.

## Agreed next direction: mark updates

Research/risk agree; coordinator authorizes engineering to prepare one reviewed
L-004 interface/harness. It is not yet a released implementation assignment.

- A required supplied integer mark is nonnegative, including zero.
- Validate an already-valuatable current state using the existing L-002 domain.
  No implicit initialization/reset or repair of missing-mark state.
- Reuse the shared event sequence: positive and greater than the previous one.
- Precedence: invalid state, invalid mark, invalid sequence, candidate overflow.
- Success changes only mark and sequence, preserves cash/quantity/funding, and
  returns checked equity using L-001. Rejection leaves inputs unchanged.
- Zero/same mark and zero holdings are valid. Sequence describes supplied order;
  it establishes no timestamp, price freshness, provenance or execution quality.
- Engineering owns exact public names/signature and reviewed scaffold.
  Tommy owns the eventual transition/tests; seniors must not write the solution.
  One short approach discussion follows the ready handoff.

Business example: cash10000, quantity2, mark3000 means opening equity16000.
After deposits totaling7000, cash17000 and equity23000. A mark3500 would yield
equity24000 and profit1000; mark2500 would yield equity22000 and profit-1000.
Cash stays17000. These are unrealized valuation changes, not cash proceeds.

A small senior-owned deposit/mark/profit composition can follow acceptance.
A generic mixed-event replay engine is deferred. No second apprentice task.

## Existing team tasks

Host local; each specialist uses its own worktree and feature branch.

- Team room: `01a0a692-cdb4-7f20-9fde-95544eaed3a1`
- Engineering: `01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941`
- Research: `01a0a6b7-14f7-79e2-9dee-48bddb4a650f`
- Risk: `01a0a6b7-2083-7d30-aeda-b8e02f781461`

## Limits

Windows Debug evidence is not exhaustive arithmetic proof, Release/sanitizer
coverage, alpha, or trading readiness. No withdrawals, trades, parsers, live
market data, persistence, brokerage access, spending or general framework.
Earlier [P001 plan](decisions/P001-replay-infrastructure.md) deferred marks;
this user-requested next-step agreement expands scope only as stated above.
