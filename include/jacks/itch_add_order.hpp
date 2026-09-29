#pragma once

#include <array>
#include <cstdint>
#include <variant>
#include <vector>

namespace jacks::itch {

enum class Side { buy, sell };

struct AddOrder {
    std::uint16_t stock_locate;
    std::uint16_t tracking_number;
    std::uint64_t timestamp_ns;
    std::uint64_t order_reference;
    Side side;
    std::uint32_t shares;
    std::array<char, 8> stock;
    std::uint32_t price_1e4;
};

enum class DecodeError { invalid_size, unexpected_type, invalid_side };
using AddOrderResult = std::variant<AddOrder, DecodeError>;

// Decode exactly one ITCH 5.0 A payload, including its type byte, without a
// transport/file envelope. Contract: learning/assignments/L-007.md.
// Validate size, then type, then side. Other fields retain their raw wire values;
// decoding does not establish market validity, book consistency or availability.
// Input is unchanged; the returned value owns all its fields.
[[nodiscard]] AddOrderResult decode_add_order(const std::vector<std::uint8_t>& payload);

} // namespace jacks::itch
