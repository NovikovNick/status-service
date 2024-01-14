#ifndef METALHEART_TELEMETRY_HISTOGRAM_H
#define METALHEART_TELEMETRY_HISTOGRAM_H

#include <atomic>
#include <map>
#include <mutex>
#include <string>
#include <vector>

#include "types.h"

namespace telemetry {

class Histogram final : public Metric {
  std::mutex                mutex_;
  double                    sum;
  std::map<double, int64_t> buckets_;

 public:
  Histogram(const MetricKey& key, const std::vector<double>& buckets)
      : Metric(key), sum(0) {
    for (auto bucket : buckets) {
      buckets_[bucket] = 0;
    }
  };

  void observe(double value);

  virtual void collect(std::string& out) override;
};

class HistogramBuilder : public MetricBuilder<Histogram, HistogramBuilder> {
  std::vector<double> buckets_;

 public:
  template <typename... Args>
  HistogramBuilder& buckets(Args&&... args) {
    (buckets_.push_back(args), ...);
    return *this;
  }

  virtual void measure(double value) override {
    build(buckets_).observe(value);
  }
};

}  // namespace telemetry

#endif  // METALHEART_TELEMETRY_HISTOGRAM_H
