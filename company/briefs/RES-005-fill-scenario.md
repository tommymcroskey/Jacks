# RES-005: explain a synthetic buy/sell lifecycle

Author: Quant Researcher agent. Date: 2026-09-19. Project: P001.
Base: `4a850fcfe0c984f2c343f8e2dba1558c405a86e8` (accepted L-004).
State: independent expected values for risk/engineering review, not executed results.

## Question and direction

Can we explain account value after funding, an explicitly supplied purchase, a
mark change, and an explicitly supplied sale, including fees? This uses Tommy's
valuation, deposit, profit, and mark components together. The missing primitive is
one checked buy/sell fill transition. A fill means an already specified execution;
this task does not decide whether or where an order would trade.

Research originally preferred a historical fixed-holding study first. Direct
discussion resolved the sequencing: synthetic lifecycle first, historical-data
semantics reviewed before historical interpretation. Synthetic correctness alone
provides no evidence of an investment edge. The data gate below preserves that
distinction without blocking useful accounting integration.

## Literal example

All values are invented. Money is integer minor CU (100 = 1 CU), quantities whole
units, prices minor CU per unit. Opening equity is fixed at 10000; opening cash
is not a deposit during the interval. Initial net deposits and sequence are zero.
The opening mark is explicitly supplied and already available. Immediate cash
settlement is assumed; no borrowing, shorting, taxes, interest or corporate actions.

| Accepted event | Sequence | Cash | Quantity | Mark | Net deposits | Equity | Total profit |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Opening snapshot | 0 | 10000 | 0 | 1000 | 0 | 10000 | 0 |
| Deposit 5000 | 1 | 15000 | 0 | 1000 | 5000 | 15000 | 0 |
| Buy 4 at execution price 1100; fee 20 | 2 | 10580 | 4 | 1000 | 5000 | 14580 | -420 |
| Observe mark 1200 | 3 | 10580 | 4 | 1200 | 5000 | 15380 | 380 |
| Sell 4 at execution price 1150; fee 20 | 4 | 15160 | 0 | 1200 | 5000 | 15160 | 160 |

Independent arithmetic:

- Buy cash: 15000 - 4 x 1100 - 20 = 10580. Equity: 10580 + 4 x 1000 = 14580.
  Buying above the current mark reduces marked equity by 400, plus the 20 fee.
- The mark rise adds 4 x (1200 - 1000) = 800 to equity, while cash stays 10580.
  The 380 profit at this point includes an unrealized change: the holdings have
  not been sold. Their mark is not a promised execution price.
- Sale cash: 10580 + 4 x 1150 - 20 = 15160. Selling below the mark reduces equity
  by 200, plus the 20 fee. There are now no holdings; equity equals cash.
- Terminal total profit: 15160 - 10000 - 5000 = 160, independently agreeing with
  4 x (1150 - 1100) - 20 - 20 = 160. Funding contributed 5000, not profit.

Fills preserve the mark and funding counter. Mark changes preserve cash, quantity
and funding. Fees affect cash once; do not subtract them again from reported profit.
Each row uses only observations available at its sequence. These synthetic sequence
numbers establish order, not real-world freshness, venue coverage or availability.

## Acceptance and ownership

Research owns these literals and their economic interpretation. Risk independently
checks them and defines rejection/boundary expectations. Engineering owns the exact
fill interface, reviewed scaffold and later fixed composition. Tommy owns the one
fill function and its tests; this document supplies neither implementation nor
apprentice test code. Current header/error precedence must be reviewed separately.

The fixed example should compare every intermediate state/equity, preserve original
inputs, and use the accepted valuation/profit components. Rejected fills must not be
presented as completed activity. No cost-basis or realized-profit subsystem is needed
to explain this flat-ending example. No generic mixed-event framework is requested.

## Gate before historical interpretation

Before selecting or interpreting a historical dataset, research/risk must agree:

1. The specific question and whether the inputs are observations, marks or executions.
2. Source, access/use terms, sample identity, instrument/venue coverage and limitations.
3. Mark policy, currency/unit scaling, timestamps/time zone, and when each input was
   available to the simulated account; never use later information silently.
4. Treatment of missing/stale/duplicate records and price adjustments/corporate actions,
   including when the chosen small scope must reject data instead of guessing.
5. Reproduction inputs/versions and an independent small expected-result check.
   If simulated trading is later authorized, execution assumptions and costs need
   their own explicit model; a valuation mark alone cannot establish a fill.

This is a future evidence gate, not authorization for data ingestion, purchases,
Nasdaq reconstruction, strategy search or empirical claims now.

Checks: independently hand-derived all rows and both terminal profit identities;
direct arithmetic verification accompanies the commit handoff. No lifecycle code
has run for this artifact. Next owners: risk for arithmetic review, engineering for
alignment with the exact ticket, coordinator for reviewed PR integration.
