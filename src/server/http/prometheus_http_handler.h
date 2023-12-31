#include <cstring>
#include <format>

#include "../service/state_service.h"
#include "../types.h"

namespace m8t {
class PrometheusHttpHandler : public Handler {
  StateService& state_service_;

 public:
  PrometheusHttpHandler(StateService& state_service) : state_service_(state_service) {}

  void handle(Buffer& buf) const {
    std::string data;

    {
      data += "# HELP uptime_in_seconds uptime_in_seconds metric\n";
      data += "# TYPE uptime_in_seconds gauge\n";
      data += std::format("uptime_in_seconds{{label=\"my_label\"}} {}\n",
                          state_service_.uptimeInSeconds());
    }

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