#include "dispatcher.h"

#include <telemetry/api.h>

#include <cassert>
#include <cstring>
#include <format>
#include <iostream>
#include <span>
#include <thread>

#include "http/parser.h"

namespace m8t {

Dispatcher::Dispatcher(http::Router&   router,  //
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
      conn.handle([t0, &server](Buffer& input, Buffer& output) {
        http::Parser    parser;
        std::span<char> data{input.data(), strlen(input.data())};
        auto            request = parser.extractData(data);

        http::Route route{request.method, std::string{request.path}};
        if (auto* c = server.router_.route(route); c != nullptr) {
          http::Response response{output};
          c->handle(request, response);
        }

        auto t1 = Clock::now();
        telemetry::summary()
            .name("status_service_http_request_duration_seconds")
            .tag("path", request.path)
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