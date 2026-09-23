#include "jacks/deposit.hpp"
#include "jacks/fill.hpp"
#include "jacks/mark.hpp"
#include "jacks/profit.hpp"

#include <iostream>

namespace {

bool same_state(const jacks::DepositState& a, const jacks::DepositState& b) {
    return a.cash_minor_units == b.cash_minor_units
        && a.quantity_units == b.quantity_units
        && a.mark_minor_units == b.mark_minor_units
        && a.net_deposits_minor_units == b.net_deposits_minor_units
        && a.last_sequence == b.last_sequence;
}

// Independent literals are from RES-005; this is a fixed demonstration, not replay.
bool show(const char* label, const jacks::DepositState& state, jacks::Equity equity,
          const jacks::DepositState& expected, std::int64_t expected_equity,
          std::int64_t expected_profit) {
    const auto result = jacks::calculate_profit(
        jacks::Equity{10000}, equity, state.net_deposits_minor_units);
    const auto* profit = std::get_if<jacks::Profit>(&result);
    if (!profit || !same_state(state, expected)
        || equity.minor_units != expected_equity
        || profit->minor_units != expected_profit) {
        std::cerr << label << ": accounting reconciliation failed; stopping\n";
        return false;
    }
    std::cout << label << ": sequence " << state.last_sequence
              << ", cash " << state.cash_minor_units
              << ", quantity " << state.quantity_units
              << ", mark " << *state.mark_minor_units
              << ", funding " << state.net_deposits_minor_units
              << ", equity " << equity.minor_units
              << ", profit " << profit->minor_units << '\n';
    return true;
}

bool unexpected(const char* label) {
    std::cerr << label << ": unexpected rejection; history incomplete\n";
    return false;
}

bool run() {
    const jacks::DepositState opening{10000, 0, 1000, 0, 0};
    const auto valuation = jacks::calculate_equity(
        opening.cash_minor_units, opening.quantity_units, opening.mark_minor_units);
    const auto* opening_equity = std::get_if<jacks::Equity>(&valuation);
    if (!opening_equity) return unexpected("Opening");
    if (!show("Opening", opening, *opening_equity, {10000, 0, 1000, 0, 0}, 10000, 0))
        return false;

    const jacks::Deposit deposit{5000, 1};
    const auto deposit_result = jacks::apply_deposit(opening, deposit);
    const auto* funded = std::get_if<jacks::DepositApplied>(&deposit_result);
    if (!funded) return unexpected("Deposit");
    if (!show("Deposit 5000", funded->state, funded->equity,
              {15000, 0, 1000, 5000, 1}, 15000, 0)) return false;

    // Predetermined buy after the opening mark; execution price is supplied separately.
    const jacks::Fill buy{jacks::FillSide::buy, 4, 1100, 20, 2};
    const auto buy_result = jacks::apply_fill(funded->state, buy);
    const auto* bought = std::get_if<jacks::FillApplied>(&buy_result);
    if (!bought) return unexpected("Buy");
    if (!show("Buy 4 at 1100, fee 20", bought->state, bought->equity,
              {10580, 4, 1000, 5000, 2}, 14580, -420)) return false;

    const jacks::MarkUpdate mark{1200, 3};
    const auto mark_result = jacks::apply_mark(bought->state, mark);
    const auto* marked = std::get_if<jacks::MarkApplied>(&mark_result);
    if (!marked) return unexpected("Mark");
    if (!show("Mark 1200", marked->state, marked->equity,
              {10580, 4, 1200, 5000, 3}, 15380, 380)) return false;

    const jacks::Fill sell{jacks::FillSide::sell, 4, 1150, 20, 4};
    const auto sell_result = jacks::apply_fill(marked->state, sell);
    const auto* sold = std::get_if<jacks::FillApplied>(&sell_result);
    if (!sold) return unexpected("Sell");
    if (!show("Sell 4 at 1150, fee 20", sold->state, sold->equity,
              {15160, 0, 1200, 5000, 4}, 15160, 160)) return false;

    // Verify earlier input snapshots still contain their original values.
    if (!same_state(opening, {10000, 0, 1000, 0, 0})
        || !same_state(funded->state, {15000, 0, 1000, 5000, 1})
        || !same_state(bought->state, {10580, 4, 1000, 5000, 2})
        || !same_state(marked->state, {10580, 4, 1200, 5000, 3})) {
        std::cerr << "Input snapshot changed\n";
        return false;
    }
    std::cout << "COMPLETE: ending cash/equity 15160; deposits 5000; profit 160.\n"
              << "Round trip: 4 * (1150 - 1100) - 20 - 20 = 160 minor CU.\n";
    return true;
}

} // namespace

int main() {
    std::cout << "Jacks synthetic trading walkthrough\n"
              << "Money: minor CU (100 = 1 CU). Whole asset units.\n"
              << "Predetermined fills and fees, immediate settlement.\n"
              << "Marks are valuations, not execution guarantees. No historical performance claim.\n";
    return run() ? 0 : 1;
}
