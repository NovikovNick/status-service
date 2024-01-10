#ifndef METALHEART_TELEMETRY_COUNTER_H
#define METALHEART_TELEMETRY_COUNTER_H

#include <atomic>
#include <string>

#include "types.h"

namespace telemetry {

class Counter final : public Metric {
  std::atomic<uint64_t> value_;

 public:
  Counter(std::string_view title, std::string_view description)
      : Metric(title, description), value_(0){};

  void increment();
  void add(uint64_t value);

  virtual void collect(std::string& out) override;
};

class CounterBuilder : public MetricBuilder<Counter, CounterBuilder> {
 public:
  virtual void measure(double value) override { build().add(value); }
};

}  // namespace telemetry

#endif  // METALHEART_TELEMETRY_COUNTER_H
