# L-001 work locations

Tommy owns `src/valuation.cpp` and `tests/valuation_test.cpp`: the real shared
component, not a duplicate learning implementation. Engineering supplied only
the unimplemented stub and skipped test placeholder. Keep your explanation and
red/green observations here.

See [the short build guide](../../../docs/build.md) for commands and
[the assignment](../../assignments/L-001.md) for the contract. Your discussion
and first missing-mark example are complete. No further restatement is required;
start the first test after scaffold peer review.

Explanation:

- jacks::calculate_equity:
- Error precedence is invalid_input, missing_mark, overflow. Without first checking if mark is missing, it is impossible to safely check whether mark input is valid, therefore to achieve this order some duplicate logic is required. Specifically, the mark must be checked against std::nullopt before checking if it is non-negative, and then later must be checked for nullopt equality again since the last check could have failed on either of the two AND conditions.
- If quantity is zero, then it is insignificant to check for any other errors, once input validity has been established, since nullopt mark on 0 quantity is deemed acceptable and quantity 0 with any integer value mark is 0. We can safely return cash since it is already non-negative and within int64_t range by data type.
-  After quantity check, can check for missing mark with simple equality check to nullopt.
- Overflow can be detected by reordering the sequence of multiplication and addition into a subtraction followed by a division and applying it to the max possible result (INT64_MAX). If r = c + q * m, then (r - c) / q = mark, giving us an in-range check.
- Finally, we can assume that our inputs are valid, our mark present, and our result in range, so we can return the ValuationResult object directly by its Equity alternative.

- test(calculate_equity):
- std::holds_alternative<T> for boolean result whether std::variant contains T, useful before using std::get which could potentially throw
- std::get<T>(var) for T object stored in variant, could potentially throw if variant holds non-T alternative
- Tests should be clear, their arguments passed directly to show intention of a specific case passed to the function being tested.
