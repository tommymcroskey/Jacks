# RISK-002: independent kickoff review

Reviewer: Risk and Validation agent. Date: 2026-09-15.
Assignment: review proposals, not implement or assess an apprentice submission.

## Exact artifacts and disposition

| Artifact | Reviewed commit | Disposition |
| --- | --- | --- |
| `company/briefs/ENG-001.md` | `1e35fdb3a47e8825a135b45eb3b4a4c935058d1a` | Accept as kickoff proposal; contract conditions below remain required before implementation. |
| `company/briefs/RES-001.md` | `99ae87cd442c689c44546af17027dc980c5a7d3d` | Accept as kickoff proposal; example arithmetic verified. |

These dispositions permit integrating explicitly labeled proposals. They do not
approve a finished contract, component, build environment, or trading strategy.
No correction to the research table is needed. Engineering explicitly identifies
its contract as work to agree before implementation; the following conditions
close that work rather than require a larger kickoff architecture.

## Evidence actually checked

Read each full file using `git show <reviewed-commit>:<artifact-path>` and inspected
its commit summary. Also read P001, the workflow, risk role, learning guidance,
and RISK-001 at `93b1e76f02faea39abef225c62245baa041440c1` for consistency.
RISK-001 is my own prior proposal and receives no independent approval here.
The local board is stale; the current team-room dispatch defines this assignment
and supersedes owner-only routine merge wording pending a durable policy update.

Independently recomputed RES-001 in PowerShell from event movements, with decimal
cash arithmetic, and compared all five output rows with the source table. Columns
below are cash, units, mark, equity, cumulative net flow, and profit, in CU where
applicable. All matched:

```text
100,0,10,100,0,0
59,4,10,99,0,-1
109,4,10,149,50,-1
109,4,12,157,50,7
120,3,12,156,50,6
```

Reproduction by hand: start with cash 100; subtract `4*10+1`; add 50; update
mark from 10 to 12; add `12-1` and reduce units from 4 to 3. At each step compute
equity as cash plus units times mark; subtract opening equity 100 and cumulative
net flow to obtain profit. Terminal cash is `100-40-1+50+12-1=120`.
Separate attribution gives `4*(12-10)-2=6`, matching `156-100-50=6`.
The deposit contributes 50 of the equity increase, with no profit contribution.

Inspected fee, flow, missing-mark, overflow, and ordering language in both briefs.
Executed two arithmetic counterexamples below in PowerShell; no ledger code or
compiler/linker test was run. Engineering's tool inventory remains an attributed
observation, not a build environment independently verified by this review.

## Required contract conditions and checks

1. **Correctness: preserve nonnegative cash after every event.** ENG-001 names
   unaffordable buys/withdrawals but should explicitly cover sale fees too. Cash
   0, one held unit, sale price 1, fee 2 produces cash `0+1-2=-1`; a long-only,
   no-borrowing core must reject it without changing state. State the invariant
   across all accepted events; do not impose an unnecessary rule that fees may
   never exceed sale proceeds when existing cash can cover the difference.

2. **Correctness/interface: cover valuation and reporting overflow.** Checking
   trade arithmetic alone is insufficient. In minor currency units, cash
   `9223372036854775807`, one held unit, and mark 1 require equity
   `9223372036854775808`, beyond signed 64-bit range; decimal arithmetic confirmed
   that witness. Specify checked products, balance updates, cumulative fees/flows,
   equity, and profit differences, including intermediate expressions. Decide
   whether an otherwise valid event with unrepresentable valuation is rejected
   atomically or accepted with a distinct valuation error. Either can be consistent;
   silent wraparound cannot. Test boundaries of every exposed calculation before
   component acceptance, without constructing a large test framework.

3. **Interface: settle initialization and unavailable values.** Adopt engineering's
   explicit unavailable equity for positive holdings with no observed mark. Zero
   holdings need no mark: equity equals cash. Profit is unavailable when a required
   endpoint equity is unavailable. Validate supplied initial state or constrain
   construction so invalid balances cannot enter. RES-001's opening cash 100 and
   RISK-001's zero-start deposit are compatible fixtures only when opening equity
   and interval flows are distinguished explicitly. Do not count opening cash as
   a new interval deposit. Define valid mark values and deposit/withdrawal magnitude
   rules. Test missing-mark and zero-holding cases separately.

4. **Correctness/interface: make ordering and failure scope unambiguous.** The
   monotonic unique sequence rule is suitable for this typed core; it can also
   identify events, without a second identifier mechanism. State whether sequence
   alone defines availability or timestamps are also validated. Test equal-time
   ordered events, repeated/decreasing sequences, and prefix stability: appending
   future marks cannot revise earlier outputs. Specify whether a failed event in
   a batch preserves the last accepted prefix or rolls back the whole batch.
   Individual rejection must preserve balances and sequencing metadata. This is
   a contract choice, not a demand for storage or replay infrastructure.

5. **Correctness: exercise execution and flow distinctions beyond the happy path.**
   Both briefs correctly charge fees once through cash and keep trade prices
   separate from marks. RES-001's equal-price sale does not test that distinction.
   Require a differing execution/mark fixture, a withdrawal leaving profit
   unchanged, a flat-price round trip losing only fees, and rejection checks for
   invalid typed values. These must be independently expected and reproduced on
   an exact implementation commit before trusting the component.

Engineering owns the short contract addressing these conditions; research checks
measurement semantics and risk reviews the resolved contract and future checks.
No apprentice implementation should start against unresolved interfaces.

## Optional work and scope limits

Long randomized sequences, fractional rounding, percentage returns, corporate
actions, and production persistence are deferred, not acceptance gates here.
No parser behavior is required from a typed integer core: malformed text, NaN,
or unknown currency strings belong to a future input adapter if one is introduced.
My RISK-001 references to malformed numbers and unsupported currencies/assets must
be read at the applicable boundary, not used to expand ENG-001's component.

**Maintainability:** the two-function boundary and proposed small test executable
are proportional; no additional framework is requested. **Preference:** no naming
or style changes requested. Engineering and research both suggest valuation as a
possible apprentice contribution; defer to engineering to reserve one assignment
after discussion. This review supplies no implementation or learning assessment.

## Handoff

Task: RISK-002. Branch: `codex/risk-002-review`. Base:
`669f15916ebaf74a29c6587c414b0de2cc8f60a4` (local `origin/main` at branch creation).
Only this review is submitted; exact submission commit accompanies the handoff.
Next owner: team room for PR transport and review-record routing, then engineering
for the contract. No blocker to proposal integration; the five conditions gate
implementation/acceptance as described above. No push, merge, or external approval
comment was performed. Changed source briefs require reconsidering affected review
conclusions against their new exact commits.
