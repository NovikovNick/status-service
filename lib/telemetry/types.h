#ifndef METALHEART_TELEMETRY_TYPES_H
#define METALHEART_TELEMETRY_TYPES_H

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

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

struct MetricKey {
  std::string                                  name;
  std::string                                  description;
  std::unordered_map<std::string, std::string> tags;
};

class MetricRegistry {
  MetricRegistry()                                 = default;
  MetricRegistry(const MetricRegistry&)            = delete;
  MetricRegistry& operator=(const MetricRegistry&) = delete;

 public:
  std::unordered_map<std::string, std::shared_ptr<Metric>> metrics_;
  static MetricRegistry&                                   instance() {
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

    std::string key{key_.name};
    if (auto it = metrics.find(key); it != metrics.end()) {
      return static_cast<Metric&>(*(it->second.get()));
    } else {
      metrics[key] = std::make_shared<Metric>(key_.name,  //
                                              key_.description,
                                              std::forward<Args>(args)...);
      return static_cast<Metric&>(*(metrics[key].get()));
    }
  };
};
}  // namespace telemetry
#endif  // METALHEART_TELEMETRY_TYPES_H
