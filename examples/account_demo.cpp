#include "jacks/profit.hpp"
#include "jacks/replay.hpp"

#include <iostream>

namespace {

const char* error_name(jacks::DepositError error) {
    switch (error) {
    case jacks::DepositError::invalid_state: return "invalid_state";
    case jacks::DepositError::invalid_amount: return "invalid_amount";
    case jacks::DepositError::invalid_sequence: return "invalid_sequence";
    case jacks::DepositError::overflow: return "overflow";
    }
    return "unknown";
}

bool show_run(const char* title, const std::vector<jacks::Deposit>& events) {
    const jacks::DepositState opening{10000, 2, 3000, 0, 0};
    const auto result = jacks::replay_deposits(opening, events);
    const auto* report = std::get_if<jacks::ReplayReport>(&result);
    if (!report) {
        std::cerr << title << ": invalid opening state; no summary available\n";
        return false;
    }
    const auto profit_result = jacks::calculate_profit(
        report->opening_equity, report->final_equity,
        report->final_state.net_deposits_minor_units);
    const auto* profit = std::get_if<jacks::Profit>(&profit_result);
    if (!profit) {
        std::cerr << title << ": profit calculation failed; no profit reported\n";
        return false;
    }

    std::cout << "\n" << title << "\n"
              << "Opening: cash 10000, holdings 2 at mark 3000, equity "
              << report->opening_equity.minor_units << "\n";
    for (const auto& step : report->accepted) {
        std::cout << "Accepted event " << step.input_index
                  << ": sequence " << step.applied.state.last_sequence
                  << ", cash " << step.applied.state.cash_minor_units
                  << ", equity " << step.applied.equity.minor_units
                  << ", net deposits " << step.applied.state.net_deposits_minor_units << "\n";
    }
    std::cout << "Status: " << (report->failure ? "INCOMPLETE" : "COMPLETE")
              << " (" << report->accepted.size() << '/' << events.size() << " events accepted)\n";
    if (report->failure) {
        std::cout << "Rejected event " << report->failure->input_index
                  << ": " << error_name(report->failure->error)
                  << "; later events were not processed\n";
    }
    std::cout << "Accepted-prefix summary: equity " << report->final_equity.minor_units
              << ", funding " << report->final_state.net_deposits_minor_units
              << ", investment profit " << profit->minor_units << "\n";
    return true;
}

} // namespace

int main() {
    std::cout << "Jacks account walkthrough\n"
              << "Synthetic data. Money is in minor CU (100 minor units = 1 CU).\n"
              << "Event indices start at zero. Holdings and mark stay fixed.\n";
    const bool complete = show_run("Two valid deposits", {{5000, 1}, {2000, 2}});
    const bool incomplete = show_run("Duplicate sequence stops replay",
        {{5000, 1}, {2000, 2}, {500, 2}, {100, 3}});
    return complete && incomplete ? 0 : 1;
}
