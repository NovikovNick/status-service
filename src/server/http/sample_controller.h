#include <chrono>
#include <cstring>

#include "controller.h"

namespace m8t::http {

class SampleController : public Controller {
 public:
  virtual void handle(const Request& request, Response& response) const {
    std::string data;

    data += "It works...\n";

    std::string packet = std::format(
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: {}\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "\r\n"
        "{}",
        data.size(),
        data);
    std::strcpy(response.buffer.data(), packet.c_str());
  };
};
}  // namespace m8t::http
