# See the account components working together

From the repository root, after configuring/building with the usual helper:

```powershell
./tools/l001.ps1 configure
./tools/l001.ps1 build
./build/l001/Debug/jacks_account_demo.exe
```

This synthetic walkthrough uses Tommy's valuation, deposit, and profit functions
and engineering's ordered replay. Research's fixed-holding example supplies the
hand-derived balances. No market data, new financial operation, or alternate
profit calculation is involved.

All money is in minor CU (100 minor units = 1 CU):

| Accepted position | Cash | Holdings value | Equity | Interval deposits | Profit |
| --- | ---: | ---: | ---: | ---: | ---: |
| Opening | 10000 | 6000 | 16000 | 0 | 0 |
| After deposit 5000 | 15000 | 6000 | 21000 | 5000 | 0 |
| After deposit 2000 | 17000 | 6000 | 23000 | 7000 | 0 |

The executable prints accepted rows and calls the real `calculate_profit` on
the final accepted equity, fixed opening equity, and net deposits. The second
scenario then submits a duplicate sequence and a later event. It must show
**INCOMPLETE**, rejection at input index 2, and only two accepted events out of
four. Its summary still describes the accepted prefix; zero profit does not
mean the whole input succeeded. The suffix is not processed.

The demo exits successfully when both scenarios can be rendered, including the
deliberate domain rejection. It is a walkthrough, not a batch ingestion CLI.
Unexpected initialization/profit failure exits nonzero without inventing a
summary. Two separate integration tests verify the complete/prefix accounting;
CTest also runs the executable as a smoke check. Existing independent economic
and risk suites continue to check the underlying trace behavior.

This illustrates a deposit-only account. With fixed holdings and mark, value
growth comes entirely from funding. Market-price changes remain a future
reviewed increment. CI builds and runs these checks automatically on PRs/main.
