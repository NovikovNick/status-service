#include "./server/dispatcher.h"
#include "./server/http/prometheus_http_handler.h"
#include "./server/http/sample_http_handler.h"
#include "./server/router.h"
#include "./server/service/state_service.h"
#include "./server/tcp/connection_pool.h"

int main(int argc, char* argv[]) {
  m8t::ConnectionPool connection_pool{8880};

  auto& state_service      = m8t::StateService::instance();
  auto  prometheus_handler = std::make_unique<m8t::PrometheusHttpHandler>(state_service);
  auto  sample_handler     = std::make_unique<m8t::SampleHttpHandler>();

  m8t::Router router;
  router.addHandler("/metrics", prometheus_handler.get());
  router.addHandler("/sample", sample_handler.get());

  m8t::Dispatcher server(router, connection_pool);

  server.start();

  return 0;
}