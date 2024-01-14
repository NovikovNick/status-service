#include "counter.h"

#include <cmath>
#include <format>

namespace telemetry {

void Counter::increment() { add(1); };

void Counter::add(uint64_t value) { value_.fetch_add(value); };

void Counter::collect(std::string& out) {
  out += std::format("# HELP {} {}\n", key_.name, key_.description);
  out += std::format("# TYPE {} counter\n", key_.name);
  out += std::format("{}", key_.name);
  collectTags(out);
  out += std::format(" {}\n", value_.load());
};
}  // namespace telemetry
