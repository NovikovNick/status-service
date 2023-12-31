#ifndef METALHEART_SERVER_ROUTER_H
#define METALHEART_SERVER_ROUTER_H

#include <string>
#include <string_view>
#include <unordered_map>

#include "types.h"

namespace m8t {
class Router final {
  std::unordered_map<std::string, Handler*> pathToHandler_;

 public:
  void addHandler(std::string_view path, Handler* handler);

  Handler* route(std::string_view path) const;
};

}  // namespace m8t
#endif  // METALHEART_SERVER_ROUTER_H