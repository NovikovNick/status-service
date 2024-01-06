#include <cmath>
#include <format>

#include "types.h"

namespace telemetry {

void Histogram::observe(double value) {
  auto lk = std::unique_lock(mutex_);
  for (auto& [backet, counter] : buckets_) {
    if (value <= backet) {
      ++counter;
      sum += value;
      return;
    }
  }
  assert(false);
};

void Histogram::collect(std::string& out) {
  size_t count = 0;

  auto lk = std::unique_lock(mutex_);

  out += std::format("# HELP {} {}\n", title_, description_);
  out += std::format("# TYPE {} histogram\n", title_);

  for (auto [backet, counter] : buckets_) {
    count += counter;
    out += std::format("{}_bucket{{le=\"{}\"}} {}\n",  //
                       title_,
                       backet,
                       counter);
  }

  out += std::format("{}_sum {}\n", title_, sum);
  out += std::format("{}_count {}\n", title_, count);
};
}  // namespace telemetry
