#include "dispatcher.h"

#include <format>
#include <iostream>
#include <thread>

#include "service/telemetry.h"

namespace m8t {

Dispatcher::Dispatcher(Router&         router,  //
                       ConnectionPool& connection_pool)
    : router_(router),  //
      connection_pool_(connection_pool){};

void Dispatcher::start() {
  using namespace std::chrono;
  using Clock                   = steady_clock;
  uint64_t request_counter      = 0;
  uint64_t request_fail_counter = 0;

  while (true) {
    auto conn = connection_pool_.acceptConnection();
    ++request_counter;

    auto t0 = Clock::now();
    std::thread([t0,
                 &server = *this,
                 &request_fail_counter,
                 conn = std::move(conn)]() mutable {
      try {
        conn.handle([&server](auto& buf) {
          std::string_view req(buf.data());

          char method[255]{'\0'};
          char path[255]{'\0'};
          std::sscanf(req.data(), "%s %s HTTP/1.1", method, path);

          std::cout << std::format("{}: {}\n", method, path);

          if (auto* handler = server.router_.route(path); handler != nullptr) {
            handler->handle(buf);
          }
        });
        auto t1 = Clock::now();
        telemetry::summary("status_service_http_request_duration_seconds",
                           "A summary of the http request duration.",
                           minutes{1},
                           0.5,
                           0.9,
                           0.95,
                           1.0)
            .observe(duration_cast<milliseconds>(t1 - t0).count() / 1000.0);
      } catch (...) {
        ++request_fail_counter;
        std::cout << "fail\n";
        telemetry::gauge("status_service_http_fail_request_count",  //
                         "Total failed http requests")
            .set(request_fail_counter);
      }
    }).detach();
    telemetry::gauge("status_service_http_request_count",  //
                     "Total http requests")
        .set(request_counter);
  }
}
}  // namespace m8t