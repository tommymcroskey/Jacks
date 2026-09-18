# Ordered deposit replay: first infrastructure increment

Base: integrated L-002 at `8869447ae433a7badd80ee6b14cc65e6efde5e1c`.
Interface accepted by research/risk at `839df68`. Implementation and executable
checks are reviewed separately; see the exact PR evidence for current acceptance.
Declaration: `include/jacks/replay.hpp`.

## Contract

`replay_deposits(opening, deposits)` consumes a typed, caller-ordered vector of
deposits. It reuses `apply_deposit`; it does not duplicate deposit arithmetic or
sort, parse, retry, persist, or process other event types.

First validate the opening snapshot, even if the event vector is empty. Cash,
quantity, any present mark, net-deposit counter and sequence must be nonnegative;
`calculate_equity` must succeed. Missing mark with zero holdings remains valid.
This runner starts a new interval: initial net deposits and last sequence must
both be zero. Resuming an existing event history is outside this interface.
Invalid opening state returns
`ReplayError::invalid_initial_state`, without a report or event failure index.

For valid initialization, capture opening state/equity once. Apply events in
the supplied order, feeding each successful returned state into the next call.
Each accepted step records its zero-based input index and complete
`DepositApplied` result. The first rejected event ends replay: retain the prior
accepted state/equity and return its index plus the original `DepositError`.
Do not include rejected or later events in the accepted trace or process the
suffix. Errors do not consume sequence numbers. An empty stream returns an empty
trace, no failure, and opening state/equity as final state/equity.

The input snapshot and vector stay unchanged. `ReplayReport` contains opening
and final snapshots/equities, accepted steps, and an optional failure. Event
sequence is available in each accepted state's `last_sequence`; do not add a
second independently maintained sequence field. Allocation/resource failures
are outside this domain-error contract; no `noexcept` guarantee is made.

## Reconciliation boundary

For this replay interval, funding equals the final net-deposit counter, because
the opening counter must be zero. Opening cash and holdings are part of opening
equity, not new deposits. This narrow initialization rule keeps the baseline
unambiguous; it does not change the broader supplied-state domain of apply_deposit.

The first runner ships state/equity traces without a dependency on `jacks_profit`.
Research compares them to literal independently derived expectations. Computed
profit is integrated only after Tommy's function is accepted. This is a deposit
replay component, not a complete ledger or investment backtester.

## Ownership and delivery

- Engineering: replay library and ordinary runner tests, CMake integration,
  automatic CI, L-003 interface/scaffold, and final integration.
- Research: `tests/replay_economic_test.cpp` executable economic scenarios with
  literal expected rows; own `tests/research/CMakeLists.txt` subdirectory defining
  `jacks_replay_economic_tests`. Engineering
  adds the root subdirectory wiring when those files are ready. No alternate
  transition/profit implementation and no apprentice unit-test replacement.
- Risk: independent adverse/boundary fixtures, accepted-prefix checks and review
  of executable evidence and CI failure propagation.
- Tommy: L-003 calculation and focused unit tests after reviewed handoff.

Baseline CI configures/builds/tests the existing C++17 project on a Windows
runner. Use hosted toolchain discovery, not developer-machine paths. Successful
workflow creation alone is not acceptance: observe an actual exact-head run and
verify that build/test/no-tests failure produces failure status. Pin external
actions/dependencies, request only needed repository permissions, and do not
change billing, secrets, or branch-protection settings. Engineering verifies
available runner permissions when implementing the workflow.

Acceptance includes the 35 existing passes, empty/invalid initialization,
deterministic caller-order traces, first failure and untouched accepted prefix,
unchanged inputs, independent economic rows, and a green integrated build/test
run. Additional platforms/configurations are separate follow-up work.

## Running the component checks

Use `tools/l001.ps1 configure`, `build`, and `test` as in the build guide.
`jacks_replay` links only the deposit/valuation libraries; `jacks_replay_tests`
checks initialization, ordered records and rejection/prefix behavior. The baseline
component run before research/risk integration had 39 passes (35 accepted tests
plus four replay tests), MSVC x64 Debug. This does not claim independent scenario
or hosted CI acceptance. Research and risk check their own expected rows against
the exact implementation commit before integration.
