#ifndef METALHEART_TELEMETRY_SUMMARY_H
#define METALHEART_TELEMETRY_SUMMARY_H

#include <chrono>
#include <mutex>
#include <queue>
#include <set>
#include <vector>

#include "types.h"

namespace telemetry {

class Summary final : public Metric {
  using Clock     = std::chrono::steady_clock;
  using Timepoint = Clock::time_point;

  std::chrono::milliseconds observation_time_;
  std::mutex                mutex_;
  std::multiset<double>     values_;
  std::queue<std::pair<Timepoint, std::set<double>::iterator>> refs_;
  std::vector<double>                                          quantiles_;

 public:
  Summary(std::string_view           title,
          std::string_view           description,
          std::chrono::milliseconds  observation_time,
          const std::vector<double>& quantiles)
      : Metric(title, description),  //
        observation_time_(observation_time),
        quantiles_(quantiles){};

  void observe(double value);

  virtual void collect(std::string& out) override;
};

class SummaryBuilder : public MetricBuilder<Summary, SummaryBuilder> {
  std::chrono::milliseconds observation_time_;
  std::vector<double>       quantiles_;

 public:
  SummaryBuilder& observation_time(std::chrono::milliseconds observation_time) {
    observation_time_ = observation_time;
    return *this;
  }

  template <typename... Args>
  SummaryBuilder& quantiles(Args&&... args) {
    (quantiles_.push_back(args), ...);
    return *this;
  }

  virtual void measure(double value) override {
    build(observation_time_, quantiles_).observe(value);
  };
};

}  // namespace telemetry

#endif  // METALHEART_TELEMETRY_SUMMARY_H
