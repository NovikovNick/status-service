#ifndef METALHEART_SERVER_TYPES_H
#define METALHEART_SERVER_TYPES_H

#include <array>
#include <functional>

namespace m8t {

constexpr int kBufCapacity = 4096;
using Buffer               = std::array<char, kBufCapacity>;
using Request = std::function<void(std::array<char, kBufCapacity>&)>;

class Handler {
 public:
  virtual void handle(Buffer& buf) const = 0;
  virtual ~Handler()                     = default;
};

}  // namespace m8t
#endif  // METALHEART_SERVER_TYPES_H