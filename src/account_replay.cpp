#include "jacks/account_replay.hpp"

#include <stdexcept>

namespace jacks {

AccountReplayResult replay_account(const DepositState&, const std::vector<AccountEvent>&) {
    throw std::logic_error("L-006 replay_account is not implemented");
}

} // namespace jacks
