# RES-003: independent deposit accounting fixtures

Author: Quant Researcher agent. Date: 2026-09-16. Project: P001 / L-002.
State: author-checked expectations awaiting risk arithmetic review.
Contract: `learning/assignments/L-002.md` at
`959718f9f4366058392380e2ea48bf0412fc6036` (this branch's base).

## Purpose and assumptions

Deposited money increases an account's value without being investment profit.
These synthetic, hand-calculated examples are reviewer evidence prepared separately
from Tommy's function and tests. They contain no market data or measured software
results. Tommy retains ownership of his implementation and test submission.

All money below is in integer minor units: 100 minor units = 1 fictional CU.
Quantity is whole asset units; mark is minor units per asset unit. For each fixture,
validate the opening snapshot, record its equity externally once, and initialize
net deposits and last sequence to zero. Opening cash is not an interval deposit.
Quantity and mark remain fixed. Each deposit below is one separate transition;
feed the prior successful result into the next call, not a batch API.

Equity is cash plus marked holdings. For independent reconciliation only:
profit = current equity - fixed opening equity - cumulative net deposits.
L-002 returns state and equity; this document does not request a profit API.

## A. Cash only, absent mark

Opening quantity is 0 and mark is absent throughout. Opening equity is 10,000.
No price is needed to value an account with no holdings.

| Event | Accepted sequence | Cash | Quantity | Mark | Equity | Net deposits | Profit |
| --- | ---: | ---: | ---: | --- | ---: | ---: | ---: |
| Opening snapshot | 0 | 10000 | 0 | absent | 10000 | 0 | 0 |
| Deposit 2500 | 1 | 12500 | 0 | absent | 12500 | 2500 | 0 |
| Deposit 750 | 3 | 13250 | 0 | absent | 13250 | 3250 | 0 |

Hand check: total funding is 2500 + 750 = 3250; terminal cash/equity is
10000 + 3250 = 13250; profit is 13250 - 10000 - 3250 = 0.
The sequence gap is valid under the contract and adds no monetary event.
In CU, the same terminal reconciliation is 132.50 - 100.00 - 32.50 = 0.

## B. Fixed holdings and available mark

Opening cash is 10,000, quantity 2, and mark 3,000. The holdings are worth
2 x 3000 = 6000 throughout; fixed opening equity is 10000 + 6000 = 16000.

| Event | Accepted sequence | Cash | Quantity | Mark | Equity | Net deposits | Profit |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| Opening snapshot | 0 | 10000 | 2 | 3000 | 16000 | 0 | 0 |
| Deposit 5000 | 1 | 15000 | 2 | 3000 | 21000 | 5000 | 0 |
| Deposit 2000 | 2 | 17000 | 2 | 3000 | 23000 | 7000 | 0 |

Hand check: the first deposit adds 5000 to cash, equity, and net deposits.
The second adds 2000 to each. Terminal equity is 17000 + 6000 = 23000;
profit is 23000 - 16000 - 7000 = 0. In CU this is 230 - 160 - 70 = 0.
The 7000 increase in account value is entirely funding. The unchanged holding's
6000 value belongs in opening and ending equity; it is not newly earned profit.

## Review requirements and limits

Risk should independently reconcile every row, unit scale, fixed opening baseline,
and cumulative funding total. Each accepted deposit must increase cash, equity,
and net deposits by exactly its amount while preserving quantity and mark.
The prior input snapshot remains unchanged even on success; the returned snapshot
is the next state. Arithmetic tables alone cannot demonstrate that software property.

These cover successful funding accounting only. Risk owns independent invalid-state,
amount, sequence, overflow, and rejection-atomicity expectations. Missing mark with
positive holdings is invalid current state under L-002, unlike valid fixture A.
No withdrawals, trades, fees, price changes, returns, or full-ledger claims follow.

Author checks: manually derived both tables; separately checked their six equity
and profit rows and four deposit deltas using direct PowerShell arithmetic. No
deposit code or tests were written or executed. To reproduce, evaluate the displayed
equity and profit identities for each row and compare adjacent cash/equity/counter
differences with the stated deposit. Peer disposition must cite the submitted hash.

Next owners: risk for arithmetic peer review; coordinator for PR integration after
review. These expectations support later exact-version implementation review and do
not approve an unseen harness, submission, or apprentice understanding.
