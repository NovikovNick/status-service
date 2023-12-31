#include "connection.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <utility>

namespace m8t {

Connection::Connection(int endpoint) : endpoint_(endpoint){};

Connection::Connection(Connection &&other) noexcept { *this = std::move(other); };

Connection &Connection::operator=(Connection &&other) noexcept {
  if (this == &other) return *this;

  endpoint_ = std::exchange(other.endpoint_, -1);

  return *this;
};

Connection::~Connection() {
  if (endpoint_ >= 0) {
    close(endpoint_);
  }
};

void Connection::handle(Request request_handler) {
  // keep track of the amount of data sent
  int bytesRead    = 0;
  int bytesWritten = 0;
  memset(buf_.data(), 0, kBufCapacity);  // clear the buffer

  // receive a message from the client (listen)
  bytesRead += recv(endpoint_, buf_.data(), kBufCapacity, 0);
  request_handler(buf_);
  bytesWritten += send(endpoint_, buf_.data(), strlen(buf_.data()), 0);
};

}  // namespace m8t