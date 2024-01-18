#include "router.h"

#include <cassert>

namespace m8t::http {

void Router::addHandler(const Route& route, Controller* controller) {
  assert(controller != nullptr);
  controllers_[route] = controller;
}

Controller* Router::route(const Route& route) const {
  auto it = controllers_.find(route);
  return it != controllers_.end() ? it->second : nullptr;
}

}  // namespace m8t::http