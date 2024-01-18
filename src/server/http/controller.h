#ifndef METALHEART_SERVER_HTTP_CONTROLLER_H
#define METALHEART_SERVER_HTTP_CONTROLLER_H

#include "types.h"

namespace m8t::http {

class Controller {
 public:
  virtual void handle(const Request& request, Response& response) const = 0;
  virtual ~Controller() = default;
};

}  // namespace m8t::http
#endif  // METALHEART_SERVER_HTTP_CONTROLLER_H
