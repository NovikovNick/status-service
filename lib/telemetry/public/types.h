#ifndef METALHEART_TELEMETRY_TYPES_H
#define METALHEART_TELEMETRY_TYPES_H

#include <atomic>
#include <format>
#include <memory>
#include <string>
#include <unordered_map>

#include "metric_key.h"

namespace telemetry {

class Metric {
 public:
  Metric(const MetricKey& key) : key_(key) {}

  virtual void collect(std::string& out) = 0;
  virtual ~Metric()                      = default;

 protected:
  MetricKey key_;

  void collectTags(std::string& out) const {
    if (!key_.tags.empty()) {
      out += '{';
      for (auto [tag, val] : key_.tags) {
        out += std::format("{}=\"{}\",", tag, val);
      }
      out[out.size() - 1] = '}';
    }
  }
};

class MetricRegistry {
  MetricRegistry()                                 = default;
  MetricRegistry(const MetricRegistry&)            = delete;
  MetricRegistry& operator=(const MetricRegistry&) = delete;

 public:
  std::unordered_map<MetricKey, std::shared_ptr<Metric>> metrics_;

  static MetricRegistry& instance() {
    static MetricRegistry inst;
    return inst;
  }
};

template <typename Metric, typename Self>
struct MetricBuilder {
  MetricKey key_;

 public:
  Self& name(std::string_view name) {
    key_.name = name;
    return static_cast<Self&>(*this);
  };

  Self& description(std::string_view description) {
    key_.description = description;
    return static_cast<Self&>(*this);
  };

  Self& tag(std::string_view key, std::string_view value) {
    key_.tags[std::string{key}] = value;
    return static_cast<Self&>(*this);
  };

  // Metric& ifNotExist(std::function<void(Self&)> cb) {
  //   cb(static_cast<Self&>(*this));
  //   return Metric{};
  // };

  virtual void measure(double value) = 0;

  virtual ~MetricBuilder() = default;

 protected:
  template <typename... Args>
  Metric& build(Args&&... args) {
    // todo: refactoring
    auto& metrics = MetricRegistry::instance().metrics_;

    if (auto it = metrics.find(key_); it != metrics.end()) {
      return static_cast<Metric&>(*(it->second.get()));
    } else {
      metrics[key_] = std::make_shared<Metric>(key_,  //
                                               std::forward<Args>(args)...);
      return static_cast<Metric&>(*(metrics[key_].get()));
    }
  }
};
}  // namespace telemetry
#endif  // METALHEART_TELEMETRY_TYPES_H
