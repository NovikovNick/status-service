#include "connection_pool.h"

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace m8t {

ConnectionPool::ConnectionPool(int port) {
  // setup a socket and connection tools
  sockaddr_in servAddr;
  bzero((char *)&servAddr, sizeof(servAddr));
  servAddr.sin_family      = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port        = htons(port);

  // open stream oriented socket with internet address
  // also keep track of the socket descriptor
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ < 0) {
    std::cerr << "Error establishing the server socket" << std::endl;
    exit(0);
  }

  // bind the socket to its local address
  int bindStatus = bind(socket_,  //
                        (struct sockaddr *)&servAddr,
                        sizeof(servAddr));
  if (bindStatus < 0) {
    std::cerr << "Error binding socket to local address" << std::endl;
    exit(0);
  }
};

Connection ConnectionPool::acceptConnection() {
  // listen for up to 5 requests at a time
  listen(socket_, 0);
  // receive a request from client using accept
  // we need a new address to connect with the client
  sockaddr_in newSockAddr;
  socklen_t   newSockAddrSize = sizeof(newSockAddr);
  // accept, create a new socket descriptor to
  // handle the new connection with client
  int endpoint = accept(socket_, (sockaddr *)&newSockAddr, &newSockAddrSize);

  if (endpoint < 0) {
    std::cerr << "Error accepting request from client!" << std::endl;
    exit(1);
  }

  return Connection{endpoint};
}

ConnectionPool::~ConnectionPool() { close(socket_); }
}  // namespace m8t