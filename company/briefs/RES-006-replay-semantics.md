# RES-006: mixed replay expectations and information boundaries

Author: Quant Researcher agent. Date: 2026-09-23. Project: P001 / proposed L-006.
Base: `f7ac08a9cc3796a54dd97dd6081caef2928733b6` (integrated L-005).
Status: independent literal expectations for review; no mixed replay execution claimed.

## What the next unit supplies

The accounting primitives can now value a snapshot, record funding, update a mark,
and apply a supplied execution. The existing replay handles deposits only. A new
mixed replay will join the accepted deposit, mark and fill functions into one
caller-ordered account history. Preserve the old replay API and existing apprentice
sources. Tommy owns the new replay function/tests; engineering owns the exact
interface/scaffold and independent fixed-call demo. Research owns these economic
expectations; risk owns rejection/boundary checks and peer validation.

New runs require opening net deposits and last sequence zero and an available,
representable opening valuation, including when the event list is empty. Zero
holdings with an absent mark is valid. Keep opening equity fixed. Return accepted
states/equities and their input indices, final accepted state/equity, and any original
typed transition error at the first rejected input. Do not sort or process a suffix
after rejection. Neither input state nor event list changes.

Profit remains outside this runner: the senior reporter uses `calculate_profit`
with fixed opening equity, final accepted equity and final net deposits. A reporting
error must be explicit and must not rewrite accepted account history. A rejected
stream is INCOMPLETE; its numbers describe only the accepted prefix.

## Reuse the accepted lifecycle

The source of all price/fee/balance arithmetic is
[RES-005](RES-005-fill-scenario.md), originally committed at
`5f92eaf39207cc3ff3975fba3f411616093a0193`. No new price interpretation is introduced.
Money is integer minor CU (100 = 1 CU), quantities whole units, prices minor CU/unit.
Opening state: cash 10000, quantity 0, mark 1000, funding 0, sequence 0; equity 10000.

| Zero-based input index | Supplied event | Accepted sequence | Expected equity | Expected cumulative funding | Externally reconciled profit |
| --- | --- | ---: | ---: | ---: | ---: |
| 0 | Deposit 5000 | 1 | 15000 | 5000 | 0 |
| 1 | Buy 4 at 1100, total fee 20 | 2 | 14580 | 5000 | -420 |
| 2 | Mark becomes 1200 | 3 | 15380 | 5000 | 380 |
| 3 | Sell 4 at 1150, total fee 20 | 4 | 15160 | 5000 | 160 |

Complete result: four accepted steps, no failure, final cash/equity 15160, quantity
0, retained mark 1200, net deposits 5000, last sequence 4. Equity growth of 5160
comprises 5000 funding and 160 profit. Fees and fill prices do not change funding.
Each trace row must also retain the complete state specified in RES-005.

One deliberate failing chronology changes only index 2 to mark 1200 with sequence
2, duplicating the accepted buy's sequence. Reject that mark with the original
`MarkError::invalid_sequence` at input index 2. Accept only indices 0 and 1. The
sell at index 3 is never processed. Final accepted state is cash 10580, quantity 4,
mark 1000, net deposits 5000, last sequence 2, equity 14580. Prefix profit is
14580 - 10000 - 5000 = -420, not the complete example's 160. Replaying only the
first two inputs must reproduce those same accepted states and equities.

An empty valid run has no accepted steps or failure and retains opening state and
equity 10000. Invalid opening has no valid report; exact result names follow the
reviewed engineering header. These are expectations, not apprentice test code.

## Explicit timing meaning and remaining capabilities

At index 1 the available mark is still 1000; the later mark 1200 cannot be used to
revalue that earlier trace row. A supplied fill's execution price is independent
of its valuation mark. Sequence provides input order only: it proves neither actual
observation availability, quote freshness, nor whether a supplied fill was feasible.
The example contains no decision algorithm, order generation or execution model.

A useful historical strategy backtester still needs separate, reviewed capabilities:
data interpretation and availability rules; a decision/order rule restricted to
available observations; an execution/fee model converting orders into asserted
fills; and reproducible run/report conventions. These are future dependencies,
not assignments authorized by this artifact. RES-005's historical-data gate remains
in force. There is no historical search, raw-data pipeline, market claim or framework
expansion here. Mixed replay makes the account history inspectable; it does not by
itself make a strategy simulation causally valid.

## Checks and handoff

Author independently reconciled the four expected equities/profits, terminal cash
and round-trip profit, and the rejected-prefix outcome using direct arithmetic.
No new replay or lifecycle code was run. Risk reviews the literals and semantics;
engineering aligns the exact L-006 interface; coordinator owns reviewed PR transport.
An executable claim requires later checks against Tommy's exact submitted commit.
