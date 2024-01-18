#ifndef METALHEART_SERVER_DISPATCHER_H
#define METALHEART_SERVER_DISPATCHER_H

#include "http/router.h"
#include "tcp/connection_pool.h"

namespace m8t {

class Dispatcher final {
  http::Router&   router_;
  ConnectionPool& connection_pool_;

 public:
  Dispatcher(http::Router&   router,  //
             ConnectionPool& connection_pool);

  void start();
};

}  // namespace m8t
#endif  // METALHEART_SERVER_DISPATCHER_H