#ifndef METALHEART_TELEMETRY_STATE_SERVICE_H
#define METALHEART_TELEMETRY_STATE_SERVICE_H

#include <telemetry/types.h>

#include <chrono>
#include <memory>
#include <string>

namespace m8t {

namespace telemetry {

::telemetry::Gauge& gauge(std::string_view title, std::string_view description);

template <typename... Q>
extern ::telemetry::Summary& summary(std::string_view          title,
                                     std::string_view          description,
                                     std::chrono::milliseconds observation_time,
                                     Q&&... quantiles);

void collect(std::string& out);
}  // namespace telemetry

}  // namespace m8t
#endif  // METALHEART_TELEMETRY_STATE_SERVICE_H