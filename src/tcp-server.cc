
#include <iostream>
#include <thread>

#include "./server/server.h"

// Server side
int main(int argc, char *argv[]) {
  // for the server, we only need to specify a port number
  if (argc != 2) {
    std::cerr << "Usage: port" << std::endl;
    exit(0);
  }
  // grab the port number
  int port = atoi(argv[1]);

  tcp::Server server(port);
  while (1) {
    auto conn = server.acceptConnection();
    std::thread t([conn = std::move(conn)]() mutable { conn.handle(); });
    t.detach();
  }
  return 0;
}