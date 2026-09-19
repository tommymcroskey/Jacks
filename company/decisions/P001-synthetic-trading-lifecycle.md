# P001 next milestone: one synthetic trading lifecycle

Date: 2026-09-19. Base: `4a850fcfe0c984f2c343f8e2dba1558c405a86e8`.
Tommy delegated the next-direction decision to the three existing specialists
after discussing raw Nasdaq reconstruction and modular mark interpretation.

## Decision and rationale

Engineering, Research and Risk explicitly agreed through direct discussion:
build one hand-checkable synthetic buy/sell lifecycle first. Research's earlier
historical-valuation-first proposal is preserved as the next data-semantics gate,
rather than discarded. Use the completed components before building a general
backtester or more isolated accounting exercises.

This increment demonstrates how a supplied executed trade affects an account.
It does not claim an order would actually have filled, implement matching,
generate a strategy, or establish profitability. Execution price and valuation
mark are separate inputs with separate meanings.

## One apprentice contribution

Engineering prepares one reviewed L-005 interface, exact error contract and
minimal scaffold. Tommy owns the new supplied-fill accounting function and its
focused tests after the ready handoff. Seniors do not write a replacement
implementation or modify his existing `src/` work.

The operation handles a supplied buy or sell, quantity, execution price and fee.
It changes cash/holdings and the shared sequence while preserving external
funding and the independently supplied valuation mark. No borrowing or short
positions. Engineering/Risk freeze the exact validity, affordability, missing-mark
and arithmetic rules in the assignment before release; this record is not a
competing low-level contract.

## Senior ownership

- Engineering: exact contract/build setup, then small scripted composition and
  report using the accepted fill function. The composition waits for Tommy's
  acceptance; no substitute implementation.
- Research: independently calculated literal account history and interpretation;
  define the eventual historical question and data requirements.
- Risk: independent contract review, balances, fees, rejection and ordering
  checks. Distinguish sequence validation from actual information availability.
- Coordinator: record agreement, review handoffs, PR transport and integration.

Useful architecture: observations -> explicit decision/execution assumptions ->
supplied fill -> account transition -> valuation/profit report.
This is an explanatory dependency chain, not a required class hierarchy.
Initially the senior scenario supplies the executions explicitly.

## Advancement and limits

Before accepting the synthetic lifecycle, independently reconcile cash,
holdings, fees, marks and profit; retain inputs on rejection; run the applicable
regression checks and show the scenario working end to end.

Before historical conclusions, Research specifies source/provenance, units,
observation availability, mark policy, missing-data behavior and treatment of
splits/distributions. Risk verifies those meanings and timing. Rawness is not
itself accuracy. No raw Nasdaq ingestion, purchase, brokerage access, generic
event framework, strategy search, execution realism or optimization is authorized.

Mark interpretation remains a separate responsibility from the account update.
A later data adapter or book reconstruction can supply marks under an explicit
policy; it must not silently change price precision or turn valuation into an
execution guarantee. Avoid speculative abstraction: build for this concrete case.
