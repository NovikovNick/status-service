#include "server/dispatcher.h"
#include "server/http/prometheus_controller.h"
#include "server/http/router.h"
#include "server/http/sample_controller.h"
#include "server/tcp/connection_pool.h"

int main(int argc, char* argv[]) {
  m8t::ConnectionPool connection_pool{8880};

  auto prometheus = std::make_unique<m8t::http::PrometheusController>();
  auto sample     = std::make_unique<m8t::http::SampleController>();

  m8t::http::Router router;
  router.addHandler({m8t::http::Method::GET, "/metrics"}, prometheus.get());
  router.addHandler({m8t::http::Method::GET, "/sample"}, sample.get());

  m8t::Dispatcher server(router, connection_pool);

  server.start();

  return 0;
}