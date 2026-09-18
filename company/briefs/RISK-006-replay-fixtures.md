# RISK-006: independent deposit replay acceptance fixtures

Author: Risk and Validation agent. Date: 2026-09-18.
Base: `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.
State: literal expectations prepared for peer review; no replay execution claimed.

## Contract and boundary

Replay starts a new run: opening net deposits and last sequence must both be zero.
The opening snapshot must otherwise satisfy the accepted valuation/state contract.
This deliberately excludes resuming a history. Validate initialization even for an
empty event list. An invalid opening returns a separate initialization error,
no accepted trace, and no failing event index; do not invent opening equity.

A valid empty list succeeds with the opening state/equity and an empty trace.
Process events in caller order, without sorting. Trace records contain only
accepted events, their zero-based input indices, full resulting state and equity.
First rejection reports its zero-based input index and the existing DepositError,
retains the accepted prefix/final accepted state and equity, and stops. Inputs
remain unchanged. The rejected event and remaining suffix never become accepted
records. Fixed opening equity is retained separately for reconciliation.

These semantics were reviewed directly with engineering. Exact API/header review
and executable verification remain separate; this document supplies expectations,
not an implementation or Tommy's L-003 tests. Research owns economic success
tables and their executable comparisons. Risk owns these failure/boundary cases.

## Literal fixtures

All money is integer minor currency units (100 = 1 CU); quantity is whole units.
State tuple `S = (cash, quantity, mark, net_deposits, last_sequence)`; `absent`
means no mark. Event tuple `(amount, sequence)`. `M = 9223372036854775807`.
Opening counters are zero except deliberately invalid initialization cases.

| ID | Opening S | Events in supplied order | Expected result |
| --- | --- | --- | --- |
| R01 | (100,0,absent,0,0) | none | Success, empty trace, same S, opening/final equity 100. |
| R02 | (0,1,absent,0,0) | none | Invalid opening; no equity/trace/event index. |
| R03 | (100,0,absent,1,0) | (1,1) | Invalid opening: nonzero starting flow counter; event not processed. |
| R04 | (100,0,absent,0,1) | none | Invalid opening: nonzero starting sequence, even with empty input. |
| R05 | (100,2,10,0,0) | (25,1), (7,1), (50,2) | Accept index 0 only: S=(125,2,10,25,1), equity 145. Reject index 1 invalid_sequence. Final state/equity equal that record. |
| R06 | (100,0,absent,0,0) | (5,3), (7,2), (11,4) | Accept index 0 only: S=(105,0,absent,5,3), equity 105. Reject index 1 invalid_sequence; never reorder to make it valid. |
| R07 | (100,0,absent,0,0) | (0,0), (5,1) | Reject index 0 invalid_amount before invalid_sequence; empty trace, final opening S/equity 100. |
| R08 | (12,0,absent,0,0) | (M,0) | Reject index 0 invalid_sequence before cash overflow; empty trace, final opening S/equity 12. |
| R09 | (0,0,absent,0,0) | (M,1), (1,2), (1,3) | Accept index 0: S=(M,0,absent,M,1), equity M. Reject index 1 overflow, retain that record/state; ignore suffix. |
| R10 | (M-2,1,1,0,0) | (1,1), (1,2) | Accept index 0: S=(M-1,1,1,1,1), equity M. Reject index 1 overflow in candidate equity; cash/counter updates alone fit. |
| R11 | (0,0,-1,0,0) | (0,0) | Invalid opening before considering the invalid event; no event index. |
| R12 | (M,1,1,0,0) | none | Invalid opening: equity unrepresentable, even with empty input. |

For each row, check the entire observable result, not merely whether it holds an
error. For valid openings, compare opening equity, trace length/order/index,
every state field and equity, failure presence/index/code, and final accepted
state/equity. Compare original input snapshot and event list after representative
success and rejection. Do not require a separate retry ritual for the pure API.

## Why these cases can falsify incorrect replay

- R05 distinguishes accepted-prefix retention from all-or-nothing rollback and
  continuation after failure. R06 catches sorting by sequence. Valid gaps are
  allowed; the first sequence 3 is not itself an error.
- Replay R05 with only its first event: its sole record must equal the first
  record in the complete failing run. The same prefix equality holds for R06,
  R09 and R10. Later events cannot revise earlier records.
- R07/R08 require real simultaneous violations, not merely test names claiming
  precedence. For R08, 12 + M exceeds M.
- R09 proves exact-limit success before the next event overflows cash/counter.
  R10 independently isolates valuation overflow: its second candidate cash is M,
  net deposits 2, and marked holdings 1, making mathematical equity M+1.
- Opening net deposits zero, nonnegative opening cash and deposits imply
  net deposits never exceed cash in a valid replay. Isolated counter overflow
  therefore belongs to apply_deposit's broader structural-input tests; do not
  fabricate an invalid runner opening to demand that unreachable replay case.

Hand checks: R05 opening equity is 100+2*10=120; accepted equity is
125+2*10=145; 145-120-25=0 profit. R06 equity is 100+5=105. For R09,
M+1 is 9223372036854775808, outside int64. R10's first equity is
(M-2)+1+1=M, representable. These are synthetic expectations, not market results.

## Executable and CI acceptance plan

Once the exact runner is available, exercise these fixtures against that head in
the risk worktree using a small separate test target or research's agreed adapter.
Reuse the runner; do not build a competing replay implementation. Record commands,
full commit, observed outputs and limits. Correct expected rejections are passing
tests; a wrong record/error must produce a nonzero test result.

Review CI at its exact head: pinned/reviewed dependencies, actual configure/build/
CTest run, existing 35 tests preserved, relevant acceptance target included, and
no skipped test presented as checked behavior. Verify one controlled incorrect
assertion fails the job and an empty test selection fails. Restore any temporary
probe and rerun the clean head; never modify apprentice work for these checks.
No billing changes/secrets required; report unavailable runner access rather than
claiming an unexecuted workflow succeeded. No new framework is requested.

Checks performed for this artifact: inspected integrated deposit/valuation and
accepted L-002; independently checked seven displayed arithmetic/boundary identities
with PowerShell decimal arithmetic. No runner or CI check has yet been executed.
No summary/profit implementation or apprentice test is supplied.

Next owners: engineering/research for contract and literal-fixture peer review;
risk for actual runner/CI validation when exact artifacts are available; coordinator
for integration. Initialization/result type names follow the eventual reviewed API.
