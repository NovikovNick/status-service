#ifndef METALHEART_TELEMETRY_GAUGE_H
#define METALHEART_TELEMETRY_GAUGE_H

#include <atomic>
#include <string>

#include "types.h"

namespace telemetry {

class Gauge final : public Metric {
  std::atomic<double> value_;

 public:
  Gauge(std::string_view title, std::string_view description)
      : Metric(title, description), value_(0.0){};

  void set(double value);

  virtual void collect(std::string& out) override;
};

class GaugeBuilder : public MetricBuilder<Gauge, GaugeBuilder> {
 public:
  virtual void measure(double value) override { build().set(value); }
};

}  // namespace telemetry

#endif  // METALHEART_TELEMETRY_GAUGE_H
