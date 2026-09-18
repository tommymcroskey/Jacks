# L-002 work locations

Tommy owns `src/deposit.cpp` and `tests/deposit_test.cpp`. The public interface is
`include/jacks/deposit.hpp`; behavior is specified in
[L-002](../../assignments/L-002.md). Engineering supplied only a throwing stub and
explicitly skipped test placeholder. Do not count that skip as deposit correctness.

After header/harness review and the short first-case discussion, replace the
placeholder with your first test. Implement the transition through TDD, reusing
`calculate_equity`. Keep the L-001 tests passing. Record actual commands/results
and brief red/green observations here; no reconstruction of past history is needed.

Use the existing [build guide](../../../docs/build.md) commands. The helper keeps
its `l001.ps1` name and `build/l001` directory for continuity, but configures/builds/
tests the whole project, including deposit. Run configure once after pulling new
targets, then build and test after edits. Engineering owns build/setup problems.

Initial scaffold result: 13 existing passes and 1 deposit skip. The unimplemented
stub deliberately throws; final domain failures must use the agreed DepositError.
Keep any first expected-value example your own; none is implemented here.

L-002 Red/Green Commit Log

fd3404b
14 - Deposit.DepositInvalidInputState (Failed)
 15 - Deposit.DepositMissingMarkState (Failed)
 16 - Deposit.DepositOverflowState (Failed)
 17 - Deposit.DepositNegativeNetDeposits (Failed)
 18 - Deposit.DepositNegativeStateSequence (Failed)
 19 - Deposit.DepositValidStateZeroDeposit (Failed)
 20 - Deposit.DepositValidStateNegativeDeposit (Failed)
 21 - Deposit.DepositValidStateNegativeDepositSequence (Failed)
 22 - Deposit.DepositValidStateLTDepositSequence (Failed)
 23 - Deposit.DepositValidStateEqualDepositSequence (Failed)
 24 - Deposit.DepositCashOverflow (Failed)
 25 - Deposit.DepositEquityOverflow (Failed)
 26 - Deposit.DepositInvalidStateInvalidAmount (Failed)
 27 - Deposit.DepositInvalidStateInvalidSequence (Failed)
 28 - Deposit.DepositInvalidStateOverflow (Failed)
 29 - Deposit.DepositInvalidAmountInvalidSequence (Failed)
 31 - Deposit.DepositInvalidSequenceOverflow (Failed)
 32 - Deposit.DepositValid (Failed)

01c5144
(edited) Deposit.DepositValid (Failed)

471b4a9
14 - Deposit.DepositInvalidInputState (Failed)
 15 - Deposit.DepositMissingMarkState (Failed)
 16 - Deposit.DepositOverflowState (Failed)
 17 - Deposit.DepositNegativeNetDeposits (Failed)
 18 - Deposit.DepositNegativeStateSequence (Failed)
 19 - Deposit.DepositValidStateZeroDeposit (Failed)
 20 - Deposit.DepositValidStateNegativeDeposit (Failed)
 21 - Deposit.DepositValidStateNegativeDepositSequence (Failed)
 22 - Deposit.DepositValidStateLTDepositSequence (Failed)
 23 - Deposit.DepositValidStateEqualDepositSequence (Failed)
 24 - Deposit.DepositCashOverflow (Failed)
 25 - Deposit.DepositEquityOverflow (Failed)
 (new) Deposit.DepositNetDepositsOverflow (Failed)
 26 - Deposit.DepositInvalidStateInvalidAmount (Failed)
 27 - Deposit.DepositInvalidStateInvalidSequence (Failed)
 28 - Deposit.DepositInvalidStateOverflow (Failed)
 29 - Deposit.DepositInvalidAmountInvalidSequence (Failed)
 31 - Deposit.DepositInvalidSequenceOverflow (Failed)
 32 - Deposit.DepositValid (Failed)

(and edited 17-29 to replace skip command with verification)

bb771d6]
(edited) 32 - Deposit.DepositValid
      Start  1: Infrastructure.Cpp17AndGoogleTestRun
 1/32 Test  #1: Infrastructure.Cpp17AndGoogleTestRun ................   Passed    0.01 sec
      Start  2: Valuation.CalculateEquityMissingMark
 2/32 Test  #2: Valuation.CalculateEquityMissingMark ................   Passed    0.01 sec
      Start  3: Valuation.CalculateEquityInvalidCash
 3/32 Test  #3: Valuation.CalculateEquityInvalidCash ................   Passed    0.01 sec
      Start  4: Valuation.CalculateEquityInvalidQuantity
 4/32 Test  #4: Valuation.CalculateEquityInvalidQuantity ............   Passed    0.02 sec
      Start  5: Valuation.CalculateEquityValidCalculation
 5/32 Test  #5: Valuation.CalculateEquityValidCalculation ...........   Passed    0.02 sec
      Start  6: Valuation.CalculateEquityInvalidMark
 6/32 Test  #6: Valuation.CalculateEquityInvalidMark ................   Passed    0.02 sec
      Start  7: Valuation.CalculateEquityOverflow
 7/32 Test  #7: Valuation.CalculateEquityOverflow ...................   Passed    0.01 sec
      Start  8: Valuation.CalculateEquityAlmostOverflow
 8/32 Test  #8: Valuation.CalculateEquityAlmostOverflow .............   Passed    0.01 sec
      Start  9: Valuation.CalculateEquityOnlyCash
 9/32 Test  #9: Valuation.CalculateEquityOnlyCash ...................   Passed    0.01 sec
      Start 10: Valuation.CalculateEquityInvalidCashMissingMark
10/32 Test #10: Valuation.CalculateEquityInvalidCashMissingMark .....   Passed    0.01 sec
      Start 11: Valuation.CalculateEquityMultiplicationOverflow
11/32 Test #11: Valuation.CalculateEquityMultiplicationOverflow .....   Passed    0.02 sec
      Start 12: Valuation.CalculateEquityPresentZeroMark
12/32 Test #12: Valuation.CalculateEquityPresentZeroMark ............   Passed    0.02 sec
      Start 13: Valuation.CalculateEquityZeroQuantityNegativeMark
13/32 Test #13: Valuation.CalculateEquityZeroQuantityNegativeMark ...   Passed    0.02 sec
      Start 14: Deposit.DepositInvalidInputState
14/32 Test #14: Deposit.DepositInvalidInputState ....................   Passed    0.03 sec
      Start 15: Deposit.DepositMissingMarkState
15/32 Test #15: Deposit.DepositMissingMarkState .....................   Passed    0.02 sec
      Start 16: Deposit.DepositOverflowState
16/32 Test #16: Deposit.DepositOverflowState ........................   Passed    0.01 sec
      Start 17: Deposit.DepositNegativeNetDeposits
17/32 Test #17: Deposit.DepositNegativeNetDeposits ..................   Passed    0.01 sec
      Start 18: Deposit.DepositNegativeStateSequence
18/32 Test #18: Deposit.DepositNegativeStateSequence ................   Passed    0.01 sec
      Start 19: Deposit.DepositValidStateZeroDeposit
19/32 Test #19: Deposit.DepositValidStateZeroDeposit ................   Passed    0.01 sec
      Start 20: Deposit.DepositValidStateNegativeDeposit
20/32 Test #20: Deposit.DepositValidStateNegativeDeposit ............   Passed    0.01 sec
      Start 21: Deposit.DepositValidStateNegativeDepositSequence
21/32 Test #21: Deposit.DepositValidStateNegativeDepositSequence ....   Passed    0.01 sec
      Start 22: Deposit.DepositValidStateLTDepositSequence
22/32 Test #22: Deposit.DepositValidStateLTDepositSequence ..........   Passed    0.02 sec
      Start 23: Deposit.DepositValidStateEqualDepositSequence
23/32 Test #23: Deposit.DepositValidStateEqualDepositSequence .......   Passed    0.01 sec
      Start 24: Deposit.DepositCashOverflow
24/32 Test #24: Deposit.DepositCashOverflow .........................   Passed    0.01 sec
      Start 25: Deposit.DepositNetDepositsOverflow
25/32 Test #25: Deposit.DepositNetDepositsOverflow ..................   Passed    0.01 sec
      Start 26: Deposit.DepositEquityOverflow
26/32 Test #26: Deposit.DepositEquityOverflow .......................   Passed    0.01 sec
      Start 27: Deposit.DepositInvalidStateInvalidAmount
27/32 Test #27: Deposit.DepositInvalidStateInvalidAmount ............   Passed    0.01 sec
      Start 28: Deposit.DepositInvalidStateInvalidSequence
28/32 Test #28: Deposit.DepositInvalidStateInvalidSequence ..........   Passed    0.01 sec
      Start 29: Deposit.DepositInvalidStateOverflow
29/32 Test #29: Deposit.DepositInvalidStateOverflow .................   Passed    0.01 sec
      Start 30: Deposit.DepositInvalidAmountInvalidSequence
30/32 Test #30: Deposit.DepositInvalidAmountInvalidSequence .........   Passed    0.01 sec
      Start 31: Deposit.DepositInvalidSequenceOverflow
31/32 Test #31: Deposit.DepositInvalidSequenceOverflow ..............   Passed    0.01 sec
      Start 32: Deposit.DepositValid
32/32 Test #32: Deposit.DepositValid ................................   Passed    0.01 sec

