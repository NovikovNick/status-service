#include <cstring>
#include <format>

#include "../service/telemetry.h"
#include "../types.h"

namespace m8t {
class PrometheusHttpHandler : public Handler {
 public:
  PrometheusHttpHandler() = default;

  void handle(Buffer& buf) const {
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
    std::strcpy(buf.data(), packet.c_str());
  };
};
}  // namespace m8t