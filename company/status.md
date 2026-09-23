# Jacks status

Updated 2026-09-23. Exact PR evidence and current handoffs supersede older snapshots.
Specialists run bounded assignments, not continuously.

## Completed foundation

Tommy's L001 valuation, L002 deposits, L003 profit, L004 marks and L005 supplied
fills are integrated (PRs #12, #19, #31, #37, #42). L005 final source:
b290dbc34fd0148ebc4cae552edacb3e6ad2a807; merge:
f7ac08a9cc3796a54dd97dd6081caef2928733b6. Engineering and Risk accepted the final
revision; hosted Windows Debug CI passed. Coordinator configured, built and
tested the actual merge with MSVC19.29 x64 Debug: 106/106 passed, no skips/failures.
Review and postintegration evidence is recorded on PR42.

Deposit-only replay, independent research/risk fixtures, account demo, CMake
maintenance and current build guide are integrated. This foundation is not yet a
strategy backtester. Shared credentials do not create independent GitHub reviewers.

## Active increment: mixed supplied-event replay

See [L006 direction](decisions/P001-mixed-event-replay.md). All three specialists
agree the next useful step is composition, not another accounting primitive.
Tommy requests difficulty similar to or greater than L005, flexibly.

| Owner | Bounded responsibility |
| --- | --- |
| Tommy | L006 mixed-event runner and behavioral tests, after reviewed scaffold release |
| Engineering | Exact interface/scaffold/build setup, mentoring, independent explicit-call lifecycle demo |
| Research | RES006 mixed ordering, literal results and timing limits, reusing RES005 |
| Risk | Contract/scaffold review and independent reconciliation/rejection checks |
| Team room | Canonical scope/status, handoffs and reviewed integration |

Scaffold release is pending its PR reviews, integration and checks. The assignment
uses existing deposit/mark/fill transitions in caller order, retains accepted
history, stops on first typed rejection, and keeps profit reporting outside the
runner. No senior replacement implementation. Adapting the demo to the new runner
waits for Tommy's acceptance. No second apprentice assignment.

Later capabilities: data/availability policy, decision/order interface,
execution/cost assumptions, and reproducible run reports. These are architectural
gaps, not simultaneous implementation assignments.

## Working agreements and limits

- Existing apprentice source/tests remain his; new authorized scaffolds are allowed.
- Tommy owns creating/writing future apprentice PRs. Agents record attributed reviews
  and may merge routine accepted changes unless he explicitly holds integration.
- One project, one apprentice assignment; feature branches, independent review,
  applicable checks and postintegration verification. No force-push/direct-main edits.
- Friday10am weekly maintenance heartbeat routes useful bounded engineering work;
  no continuous company loop or duplicate tasks.
- Four merged remote Research branches remain undeleted after automatic review
  rejected relayed deletion authority; direct owner answer remains pending.
  Do not retry via another agent/transport. Historical unmerged reviews remain.
- Evidence is Windows Debug, not exhaustive proof, Release/sanitizer coverage,
  alpha or execution realism. No purchases, brokerage, raw Nasdaq pipeline,
  generic strategy framework, borrowing, shorts or optimization in this increment.

## Existing tasks

Host local; specialists use their own worktrees and codex/ branches.

- Team room: 01a0a692-cdb4-7f20-9fde-95544eaed3a1
- Engineering: 01a0a6b7-0c65-7b43-a5e1-d8f0f80cf941
- Research: 01a0a6b7-14f7-79e2-9dee-48bddb4a650f
- Risk: 01a0a6b7-2083-7d30-aeda-b8e02f781461
