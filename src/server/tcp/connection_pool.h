#ifndef METALHEART_SERVER_TCP_CONNECTION_POOL_H
#define METALHEART_SERVER_TCP_CONNECTION_POOL_H

#include "connection.h"

namespace m8t {

class ConnectionPool final {
  int socket_;

 public:
  explicit ConnectionPool(int port);

  Connection acceptConnection();

  ~ConnectionPool();
};

}  // namespace m8t
#endif  // METALHEART_SERVER_TCP_CONNECTION_POOL_H