# See the accounting components handle a trade lifecycle

From the repository root in PowerShell 7:

```powershell
./tools/l001.ps1 all
./build/l001/Debug/jacks_trading_demo.exe
```

This senior-owned executable calls the accepted valuation, deposit, fill, mark
and profit functions directly. It does not implement L-006's event dispatch or
loop. Every row is checked against the independent
[RES-005 example](../company/briefs/RES-005-fill-scenario.md), including all state
fields, equity and profit. It exits nonzero on unexpected rejection, reconciliation
failure or changes to earlier snapshots. CTest runs it as `TradingDemo.Reconciles`.

Starting equity is 10000 minor CU. Deposit 5000; buy four units at 1100 with fee
20; observe mark 1200; sell the four units at 1150 with fee 20. Equity at those
steps is 15000, 14580, 15380 and 15160. Final profit is 160 after both fees;
funding contributed 5000 and is excluded from profit. The mark-time profit of
380 includes unrealized value and is not the final cash profit.

These are predetermined synthetic fills, with a supplied opening mark available
before the buy and a later mark before the sale. They demonstrate accounting,
not a decision rule, executable market prices, realistic timing or an investment
edge. No matching, spread/slippage estimation, raw data ingestion or strategy
search is included. Historical work remains behind the documented data gate.
