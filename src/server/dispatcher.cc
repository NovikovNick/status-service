#include "dispatcher.h"

#include <telemetry/api.h>

#include <format>
#include <iostream>
#include <thread>

#include "llhttp.h"

namespace {

class HttpRequest {
  llhttp_t          parser;
  llhttp_settings_t settings;

 public:
  std::string path;

  HttpRequest() {
    llhttp_settings_init(&settings);

    settings.on_url = HttpRequest::onUrl;

    llhttp_init(&parser, HTTP_BOTH, &settings);
    parser.data = this;
  }

  static int onUrl(llhttp_t* p, const char* at, size_t len) try {
    auto self  = static_cast<HttpRequest*>(p->data);
    self->path = std::string{at, len};

    std::cout << std::format("Path: {}\n", self->path);

    return HPE_OK;
  } catch (...) {
    return HPE_INTERNAL;
  }

  void extractData(const char* req, int len) {
    enum llhttp_errno err = llhttp_execute(&parser, req, len);
    if (err != HPE_OK) {
      std::cout << std::format("Parse error: {} {}\n",  //
                               llhttp_errno_name(err),
                               parser.reason);
    }
  }
};

}  // namespace

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

        HttpRequest request;
        request.extractData(req.data(), req.size());

        if (auto* handler = server.router_.route(request.path);
            handler != nullptr) {
          handler->handle(buf);
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