#include "dispatcher.h"

#include <telemetry/api.h>

#include <format>
#include <iostream>
#include <thread>

namespace m8t {

Dispatcher::Dispatcher(Router&         router,  //
                       ConnectionPool& connection_pool)
    : router_(router),  //
      connection_pool_(connection_pool){};

void Dispatcher::start() {
  using namespace std::chrono;
  using Clock              = steady_clock;
  uint64_t request_counter = 0;

  while (true) {
    auto conn = connection_pool_.acceptConnection();
    ++request_counter;

    auto t0 = Clock::now();
    std::thread([t0, &server = *this, conn = std::move(conn)]() mutable {
      conn.handle([t0, &server](auto& buf) {
        std::string_view req(buf.data());

        char method[255]{'\0'};
        char path[255]{'\0'};
        std::sscanf(req.data(), "%s %s HTTP/1.1", method, path);

        std::cout << std::format("{}: {}\n", method, path);

        if (auto* handler = server.router_.route(path); handler != nullptr) {
          handler->handle(buf);
        }

        auto t1 = Clock::now();
        telemetry::summary()
            .name("status_service_http_request_duration_seconds")
            .tag("path", path)
            .description("A summary of the http request duration.")
            .observation_time(minutes{1})
            .quantiles(0.5, 0.9, 0.95, 1.0)
            .measure(duration_cast<milliseconds>(t1 - t0).count() / 1e3);
      });
    }).detach();

    telemetry::gauge()
        .name("status_service_http_request_count")
        .description("Total http requests")
        .measure(request_counter);
  }
}
}  // namespace m8t