#include "connection.h"

#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <utility>

namespace tcp {

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

void Connection::handle() {
  // keep track of the amount of data sent
  int bytesRead, bytesWritten = 0;
  while (1) {
    // receive a message from the client (listen)
    std::cout << "Awaiting client response..." << std::endl;
    memset(&buf_, 0, sizeof(buf_));  // clear the buffer

    bytesRead += recv(endpoint_, (char *)&buf_, sizeof(buf_), 0);
    if (!strcmp(buf_, "exit")) {
      std::cout << "Client has quit the session" << std::endl;
      break;
    }
    std::cout << "Client: " << buf_ << std::endl;
    std::cout << ">";

    std::string data = "server answer";

    memset(&buf_, 0, sizeof(buf_));  // clear the buffer
    strcpy(buf_, data.c_str());
    if (data == "exit") {
      // send to the client that server has closed the connection
      send(endpoint_, (char *)&buf_, strlen(buf_), 0);
      break;
    }
    // send the message to client
    bytesWritten += send(endpoint_, (char *)&buf_, strlen(buf_), 0);
  }

  std::cout << "********Session********" << std::endl;
  std::cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead
            << std::endl;

  std::cout << "Connection closed..." << std::endl;
};

}  // namespace tcp