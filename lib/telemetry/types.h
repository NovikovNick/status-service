#ifndef METALHEART_TELEMETRY_TYPES_H
#define METALHEART_TELEMETRY_TYPES_H

#include <atomic>
#include <cassert>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <vector>

namespace telemetry {

class Metric {
 protected:
  const std::string title_, description_;

 public:
  Metric(std::string_view title, std::string_view description)
      : title_(title),             //
        description_(description)  //
  {}

  virtual void collect(std::string& out) = 0;
  virtual ~Metric()                      = default;
};

class Counter final : public Metric {
  std::atomic<uint64_t> value_;

 public:
  Counter(std::string_view title, std::string_view description)
      : Metric(title, description), value_(0){};

  void increment();
  void add(uint64_t value);

  virtual void collect(std::string& out) override;
};

class Gauge final : public Metric {
  std::atomic<double> value_;

 public:
  Gauge(std::string_view title, std::string_view description)
      : Metric(title, description), value_(0.0){};

  void set(double value);

  virtual void collect(std::string& out) override;
};

class Histogram final : public Metric {
  std::mutex                mutex_;
  double                    sum;
  std::map<double, int64_t> buckets_;

 public:
  template <typename... B>
  Histogram(std::string_view title, std::string_view description, B... buckets)
      : Metric(title, description), sum(0) {
    static_assert((std::is_same<B, double>::value && ...));
    ((buckets_[buckets] = 0), ...);
  };

  void observe(double value);

  virtual void collect(std::string& out) override;
};

class Summary final : public Metric {
  using Clock     = std::chrono::steady_clock;
  using Timepoint = Clock::time_point;

  std::chrono::milliseconds observation_time_;
  std::mutex                mutex_;
  std::multiset<double>     values_;
  std::queue<std::pair<Timepoint, std::set<double>::iterator>> refs_;
  std::vector<double>                                          quantiles_;

 public:
  template <typename... Q>
  Summary(std::string_view          title,
          std::string_view          description,
          std::chrono::milliseconds observation_time,
          Q... quantiles)
      : Metric(title, description),  //
        observation_time_(observation_time) {
    static_assert((std::is_same<Q, double>::value && ...));
    assert(((quantiles >= 0 && quantiles <= 1) && ...));
    (quantiles_.push_back(quantiles), ...);
  };

  void observe(double value);

  virtual void collect(std::string& out) override;
};

}  // namespace telemetry
#endif  // METALHEART_TELEMETRY_TYPES_H
