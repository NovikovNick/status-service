#include <chrono>
#include <cstring>

#include "../types.h"

namespace m8t {
class SampleHttpHandler : public Handler {
 public:
  void handle(Buffer& buf) const {
    std::string data;
    { data += "It works...\n"; }

    std::string packet = std::format(
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: {}\r\n"
        "Content-Type: text/plain; charset=utf-8\r\n"
        "\r\n"
        "{}",
        data.size(),
        data);
    std::strcpy(buf.data(), packet.c_str());
  };
};
}  // namespace m8t