#ifndef METALHEART_SERVER_HTTP_REQUEST_H
#define METALHEART_SERVER_HTTP_REQUEST_H

#include <array>
#include <string>

#include "../types.h"

namespace m8t {

namespace http {

enum class Method { GET, POST };

struct Request final {
  Method           method;
  std::string_view path;
};

struct Response final {
  Buffer& buffer;
};

}  // namespace http

}  // namespace m8t
#endif  // METALHEART_SERVER_HTTP_REQUEST_H