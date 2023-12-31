#ifndef METALHEART_SERVER_DISPATCHER_H
#define METALHEART_SERVER_DISPATCHER_H

#include "router.h"
#include "tcp/connection_pool.h"

namespace m8t {

class Dispatcher final {
  Router&         router_;
  ConnectionPool& connection_pool_;

 public:
  Dispatcher(Router&         router_,  //
             ConnectionPool& connection_pool_);

  void start();
};

}  // namespace m8t
#endif  // METALHEART_SERVER_DISPATCHER_H