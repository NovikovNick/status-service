#ifndef METALHEART_SERVER_HTTP_ROUTER_H
#define METALHEART_SERVER_HTTP_ROUTER_H

#include <string>
#include <unordered_map>

#include "controller.h"
#include "types.h"

namespace m8t::http {

struct Route final {
  Method      method;
  std::string path;

  bool operator==(const Route& other) const {
    return method == other.method && path == other.path;
  }
};
}  // namespace m8t::http

namespace std {
template <>
struct hash<m8t::http::Route> {
  size_t operator()(const m8t::http::Route& route) const {
    return hash<string>()(route.path);
  }
};
}  // namespace std

namespace m8t::http {

class Router final {
  std::unordered_map<Route, Controller*> controllers_;

 public:
  void addHandler(const Route&, Controller*);

  Controller* route(const Route&) const;
};
}  // namespace m8t::http
#endif  // METALHEART_SERVER_HTTP_ROUTER_H