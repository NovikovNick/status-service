#include <cstring>
#include <format>

#include "../service/telemetry.h"
#include "controller.h"

namespace m8t::http {

class PrometheusController : public Controller {
 public:
  void handle(const Request& request, Response& response) const {
    std::string data;

    telemetry::collect(data);

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