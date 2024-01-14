#ifndef METALHEART_TELEMETRY_METRIC_KEY_H
#define METALHEART_TELEMETRY_METRIC_KEY_H

#include <string>
#include <unordered_map>

namespace telemetry {
struct MetricKey {
  std::string                                  name;
  std::string                                  description;
  std::unordered_map<std::string, std::string> tags;

  bool operator==(const MetricKey& other) const {
    return name == other.name        //
               ? tags == other.tags  //
               : false;
  };
};

}  // namespace telemetry

namespace std {
template <>
struct hash<telemetry::MetricKey> {
  size_t operator()(const telemetry::MetricKey& key) const {
    return hash<string>()(key.name);
  }
};
}  // namespace std

#endif  // METALHEART_TELEMETRY_METRIC_KEY_H