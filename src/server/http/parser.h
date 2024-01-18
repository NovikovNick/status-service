#ifndef METALHEART_SERVER_HTTP_PARSER_H
#define METALHEART_SERVER_HTTP_PARSER_H

#include <cassert>
#include <format>
#include <iostream>
#include <span>
#include <string>

#include "llhttp.h"
#include "types.h"

namespace m8t::http {

class Parser {
  llhttp_t          parser;
  llhttp_settings_t settings;

 public:
  Request request;

  Parser() {
    llhttp_settings_init(&settings);

    settings.on_url = Parser::onUrl;

    llhttp_init(&parser, HTTP_BOTH, &settings);
    parser.data = this;
  }

  static int onUrl(llhttp_t* p, const char* at, size_t len) try {
    auto self = static_cast<Parser*>(p->data);
    assert(self != nullptr);

    self->request.path = std::string_view{at, len};

    return HPE_OK;
  } catch (...) {
    return HPE_INTERNAL;
  }

  Request extractData(std::span<char> buf) {
    llhttp_errno err = llhttp_execute(&parser, buf.data(), buf.size());
    if (err != HPE_OK) {
      std::cout << std::format("Parse error: {} {}\n",  //
                               llhttp_errno_name(err),
                               parser.reason);
      std::cout << std::format("{}\n", std::string{buf.data(), buf.size()});
    }

    std::cout << std::format("{} {}: {}\n",  //
                             parser.method,
                             request.path,
                             buf.size());

    request.method = parser.method == 1  // tmp
                         ? http::Method::GET
                         : http::Method::POST;
    return request;
  }
};
}  // namespace m8t::http
#endif  // METALHEART_SERVER_HTTP_PARSER_H