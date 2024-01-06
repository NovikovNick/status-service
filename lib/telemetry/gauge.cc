#include <cmath>
#include <format>

#include "types.h"

namespace telemetry {

void Gauge::set(double value) { value_.store(value); };

void Gauge::collect(std::string& out) {
  out += std::format("# HELP {} {}\n", title_, description_);
  out += std::format("# TYPE {} gauge\n", title_);
  out += std::format("{} {}\n", title_, value_.load());
};
}  // namespace telemetry
