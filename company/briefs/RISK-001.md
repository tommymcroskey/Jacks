# RISK-001: validation proposal for P001

Status: proposal for kickoff review; no implementation or independent review exists.
Author: Risk and Validation agent. Scope: one currency, cash, one long asset,
synthetic executed trades, fees, and external cash flows.

## What would make the ledger trustworthy?

A ledger records events and the resulting balances. Reconciliation means checking
that those balances agree with an independently calculated explanation. Equity is
cash plus the current marked value of the asset. A mark is the price used to value
an existing position; it need not equal a trade's execution price.

Propose an explicit contract before coding: cash and fees in currency units,
quantity in whole shares initially, and prices in currency units per share. Use
exact cents for these fixtures; engineering should specify representation,
overflow handling, and rounding before fractional amounts are allowed. Start at
zero cash and shares. Define signed external flow as deposits minus withdrawals.
Fees are nonnegative costs attached once to executed trades. Exclude borrowing,
shorts, interest, splits, and dividends from this first contract.

Events need a unique ID and a total order, such as timestamp plus sequence number.
Equal timestamps require that explicit tie-breaker; reject ambiguous or backward
order. Only marks already available in that order may value a position. Require a
mark before reporting nonzero holdings' equity; never silently use zero or a
future price. A trade changes cash and quantity; a separate mark changes valuation.

## Independent expected values

These are constructed arithmetic expectations, not measured software results.
At each row check equity = cash + shares × latest available mark. With no opening
capital, cumulative profit = equity − cumulative net external flows. This measures
money earned or lost, not a percentage return.

| Ordered event | Cash | Shares | Mark | Equity | Net flows | Profit |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Deposit 1,000 | 1,000 | 0 | absent | 1,000 | 1,000 | 0 |
| Mark 10 | 1,000 | 0 | 10 | 1,000 | 1,000 | 0 |
| Buy 20 at 10, fee 2 | 798 | 20 | 10 | 998 | 1,000 | −2 |
| Mark 12 | 798 | 20 | 12 | 1,038 | 1,000 | 38 |
| Deposit 100 | 898 | 20 | 12 | 1,138 | 1,100 | 38 |
| Sell 5 at 11, fee 1 | 952 | 15 | 12 | 1,132 | 1,100 | 32 |
| Withdraw 50 | 902 | 15 | 12 | 1,082 | 1,050 | 32 |

An independent terminal calculation gives cash = 1,000 − 200 − 2 + 100 + 55 − 1
− 50 = 902; shares = 20 − 5 = 15; equity = 902 + 180 = 1,082.
Profit also equals 40 from the price rise, minus 5 from selling below the mark,
minus 3 in fees = 32. Keeping mark and execution price distinct catches a hidden
valuation assumption.

## Required falsification checks

Falsification checks deliberately try to expose a wrong result.

- **Balances:** independently sum signed cash movements and share movements;
  compare every intermediate balance, not only the final equity.
- **Fees and flows:** a flat-price buy/sell round trip loses exactly its fees;
  deposits and withdrawals change equity by their amounts and leave profit
  unchanged. A mark change with zero holdings changes no value.
- **Invalid events:** reject overselling, insufficient cash including fees,
  excessive withdrawals, duplicate IDs, nonpositive trade quantities/prices,
  negative fees, unsupported currencies/assets, malformed numbers, and overflow.
  Reject zero/negative flow magnitudes under separate deposit/withdrawal types.
  Failure leaves all balances and event history unchanged.
- **Timing:** recomputing any event prefix must reproduce its original outputs;
  appending a later price cannot alter earlier equity. Test equal-time sequence
  order, missing marks, and rejected backward events explicitly.

Trust requires an agreed contract, expected values authored independently of the
implementation, these checks passing on an exact commit, and another specialist
reproducing the documented command. Agreement between agents alone is insufficient.
Missing contract choices or failed reconciliations block component acceptance;
resolve them with a recorded decision or corrected artifact and rerun evidence.
Optional later stress tests include long sequences and fractional rounding. Passing
tiny fixtures establishes neither production readiness nor investment performance.

## Next work and handoff

Independent next task: risk prepares additional hand-calculated rejection and
timing fixtures after kickoff acceptance, without implementing the component.
Possible apprentice contribution: a small balance-update function, scoped by
engineering after the introductory discussion and apprentice restatement. This
brief issues no assignment and supplies no implementation.

Team room: route this brief to engineering and research for actual cross-review of
contract feasibility and accounting expectations. Accept the proposal when required
checks are actionable and disagreements are recorded. Proposed board state:
RISK-001 in review. Foundation dependency resolved: the team room reports owner
merge of PR #1; fetched `origin/main` at
`669f15916ebaf74a29c6587c414b0de2cc8f60a4` contains the PR workflow.
Branch: `codex/risk-001-kickoff`; artifact: `company/briefs/RISK-001.md`.
The final handoff supplies the exact commit and checks. Next owner: team room;
implementation remains gated on kickoff review.
