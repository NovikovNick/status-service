#include "counter.h"

#include <cmath>
#include <format>

namespace telemetry {

void Counter::increment() { add(1); };

void Counter::add(uint64_t value) { value_.fetch_add(value); };

void Counter::collect(std::string& out) {
  out += std::format("# HELP {} {}\n", title_, description_);
  out += std::format("# TYPE {} counter\n", title_);
  out += std::format("{} {}\n", title_, value_.load());
};
}  // namespace telemetry
