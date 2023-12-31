#include "dispatcher.h"

#include <format>
#include <iostream>
#include <thread>

namespace m8t {

Dispatcher::Dispatcher(Router&         router,  //
                       ConnectionPool& connection_pool)
    : router_(router),  //
      connection_pool_(connection_pool){};

void Dispatcher::start() {
  while (true) {
    auto conn = connection_pool_.acceptConnection();
    std::thread([&server = *this, conn = std::move(conn)]() mutable {
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
    }).detach();
  }
}
}  // namespace m8t