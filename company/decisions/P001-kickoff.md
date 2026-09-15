# P001 kickoff: accepted valuation boundary

Date: 2026-09-15. Recorded by the team-room coordinator from actual specialist reviews.

## Evidence

Kickoff proposals were accepted as proposals and merged through PRs #3, #4, and #5.
Engineering review c59d896f615cda3e6da38f6d152c5db0c5142047, research review
1f1adb830a7c07da484be11027e5eab0499a2571, and risk review
b031feef692cc0bfbbe536588da24945a08566cc record the exact source commits and checks.
Those records are merged through PRs #6, #7, and #8. Policy and registration were
independently reviewed and merged through PR #2.

Research and risk then reviewed ENG-002 and L-001 at
c59d896f615cda3e6da38f6d152c5db0c5142047. Both accepted the provisional valuation
contract; their attributed review is recorded on PR #6. Research explicitly
withdrew its positive-only mark proposal. No implementation is accepted here.

## Accepted L-001 contract

- One snapshot: cash in minor currency units, quantity in whole asset units,
  and an optional mark in minor currency units per asset unit; C++17/int64 storage.
- Cash and quantity must be nonnegative. A supplied mark must be nonnegative,
  even when quantity is zero. Zero is a valid mark, not a missing price.
- With zero holdings, a missing mark permits cash-only equity. Positive holdings
  without a mark return missing_mark.
- Return an equity value or an explicit named error, not a numeric sentinel.
  Error precedence: invalid_input, then missing_mark, then overflow.
- Check representability before signed multiplication/addition. No partial result
  on error. Inputs are unchanged. This function neither calculates profit nor
  processes trades, fees, identifiers, symbols, or event order.
- Minimal shared types and exact function signature belong to engineering's harness
  review. No tables, template framework, or overloaded financial arithmetic is needed.

Tommy owns the valuation test and implementation. His missing-mark test scenario
(cash 10000, quantity 40, mark absent -> missing_mark) was accepted by engineering.
He correctly identified zero investment profit from a pure deposit. These are
specific observed explanations, not a claim of broader mastery.

## Teaching and infrastructure

Tommy prefers terminal-first work and test-driven development. Agree on invariants
and independently justified expected results, observe a meaningful failing test,
implement the smallest correct change, then refactor. Apply this to senior
engineering work too; do not substitute implementation-mirroring tests for evidence.

Engineering owns the CMake/CTest/GoogleTest harness, immutable dependency version,
build guide, and compile/link/test verification. GoogleTest is test-only. A skipped
placeholder is not a passing valuation test. Do not implement Tommy's accepted test
or the valuation behavior for him. Explain layout briefly and answer specific
questions rather than requiring repeated contract essays.

The shared library and its tests live in the normal include/src/tests layout.
The learning assignment points to those files; it does not duplicate production code.
Engineering supplies the final exact paths with the reviewed harness.

## Still gated

Release implementation work once the harness and exact API have independent review
and verified terminal commands. Full ledger work still requires an explicit contract
covering nonnegative cash after every event (including sale fees), checked accounting
counters, event acceptance versus valuation failure, initialization, sequencing and
batch-prefix behavior. It is not implied by this valuation-only decision.

No strategy, empirical market result, trading readiness, or autonomous schedule is
approved. The team may review and merge ordinary bounded work under Tommy's standing
authority, retaining spending and trading limits and preserving reversible history.
