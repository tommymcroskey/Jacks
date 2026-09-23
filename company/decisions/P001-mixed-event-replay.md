# P001: mixed supplied-event replay (L006)

Date: 2026-09-23. Base: f7ac08a9cc3796a54dd97dd6081caef2928733b6.
Tommy requests the next useful assignment at similar or greater difficulty than L005,
flexibly, and coordinated senior progress.

## Assessment and decision

Engineering, Research and Risk agree: the accepted accounting primitives cover
our current one-currency, one-long-asset synthetic model. No additional accounting
primitive is required before connecting them. L006 is a bounded typed replay of
supplied deposits, mark updates and fills. It uses existing transitions rather
than duplicating their validation or arithmetic. Existing deposit-only replay and
apprentice source/tests remain unchanged.

This is a step toward a backtester, not a complete strategy backtester. Still
needed later: data normalization and information-availability rules, a decision
interface, an explicit execution/cost model that produces fills, and reproducible
run/report tooling. Marks remain valuations; they do not establish executable
prices. Corporate actions, borrowing and other accounting extensions require an
actual later scope, not speculative functions now.

## One apprentice assignment

Engineering owns the exact L006 header, assignment, minimal throwing scaffold and
build setup, reviewed independently before release. Tommy owns implementation,
behavioral tests, and his PR description/creation. Invite a short restatement and
approach before implementation; do not supply his runner or test solution.

The agreed contract shape is a new run with zero opening funding/sequence
counters, opening validation even for no events, typed deposit/mark/fill inputs in
caller order, one shared sequence, an accepted-only trace, and first rejection
reported with its input index and original typed error. Rejection retains the
last accepted snapshot and does not process later events. Inputs stay unchanged.
No sorting, repair, execution inference or generic event framework.

Profit calculation stays outside the runner in senior reporting, using fixed
opening equity and accepted interval funding. The engineering header/assignment
will define exact types and precedence; this record is not a competing interface.

Difficulty comes from composing three operations, typed error propagation,
trace consistency and failure boundaries. It is broader than L005 in integration
responsibility, without artificially increasing arithmetic or test volume.

## Senior work and dependencies

- Engineering: reviewed scaffold and mentoring; a separate explicit-call RES005
  lifecycle demo may use the accepted primitives now. Adapting that demo to the
  new runner waits for Tommy's accepted L006.
- Research: RES006 references existing RES005 literals, supplies exact mixed
  ordering and rejected-prefix expectations, and explains timing limitations.
- Risk: independently reviews the contract/scaffold and literal reconciliation,
  preserving the distinction between event order and real information timing.
- Coordinator: bounded handoffs, canonical records and reviewed integration.

No new apprentice assignment runs concurrently. Seniors do not implement a
replacement runner while waiting. Normal branch/PR/review/check protocol applies;
Tommy owns future apprentice PR authoring. No historical-data acquisition, trading,
strategy search, or generalized execution engine in this increment.

## Completion

Release L006 only after reviewed scaffold integration and actual build checks.
The later implemented runner must reconcile the RES005 lifecycle, preserve inputs
and accepted prefixes, retain original typed failures, and pass proportionate
tests and independent review. Synthetic results are accounting evidence only.
