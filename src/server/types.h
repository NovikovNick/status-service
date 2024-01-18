#ifndef METALHEART_SERVER_TYPES_H
#define METALHEART_SERVER_TYPES_H

#include <array>
#include <functional>

namespace m8t {

constexpr int kBufCapacity = 4096;
using Buffer               = std::array<char, kBufCapacity>;
using Request              = std::function<void(Buffer&, Buffer&)>;

}  // namespace m8t
#endif  // METALHEART_SERVER_TYPES_H