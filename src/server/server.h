#ifndef METALHEART_TCP_SERVER_H
#define METALHEART_TCP_SERVER_H

#include "connection.h"

namespace tcp {

class Server {
  int socket_;

 public:
  Server(int port);

  Connection acceptConnection();

  ~Server();
};

}  // namespace tcp
#endif  // METALHEART_TCP_SERVER_H