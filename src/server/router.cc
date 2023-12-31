#include "router.h"

#include <cassert>

namespace m8t {

void Router::addHandler(std::string_view path, Handler* handler) {
  assert(handler != nullptr);
  pathToHandler_[std::string{path}] = handler;
}

Handler* Router::route(std::string_view path) const {
  auto it = pathToHandler_.find(std::string{path});
  return it != pathToHandler_.end() ? it->second : nullptr;
}

}  // namespace m8t