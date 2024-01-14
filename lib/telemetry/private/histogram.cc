#include "histogram.h"

#include <cassert>
#include <cmath>
#include <format>

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

  out += std::format("# HELP {} {}\n", key_.name, key_.description);
  out += std::format("# TYPE {} histogram\n", key_.name);

  for (auto [backet, counter] : buckets_) {
    count += counter;
    out += std::format("{}_bucket", key_.name);
    key_.tags["le"] = std::to_string(backet);
    collectTags(out);
    out += std::format(" {}\n", counter);
  }

  out += std::format("{}_sum {}\n", key_.name, sum);
  out += std::format("{}_count {}\n", key_.name, count);
};
}  // namespace telemetry
