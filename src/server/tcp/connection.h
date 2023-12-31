#ifndef METALHEART_SERVER_CONNECTION_H
#define METALHEART_SERVER_CONNECTION_H

#include <array>
#include <functional>

#include "../types.h"

namespace m8t {

class Connection final {
  std::array<char, kBufCapacity> buf_;
  int                            endpoint_;

 public:
  explicit Connection(int endpoint_);

  Connection(const Connection &)            = delete;
  Connection &operator=(const Connection &) = delete;

  Connection(Connection &&) noexcept;
  Connection &operator=(Connection &&) noexcept;

  ~Connection();

  void handle(Request request_handler);
};

}  // namespace m8t
#endif  // METALHEART_SERVER_CONNECTION_H