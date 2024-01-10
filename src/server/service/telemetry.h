#ifndef METALHEART_TELEMETRY_STATE_SERVICE_H
#define METALHEART_TELEMETRY_STATE_SERVICE_H

#include <chrono>
#include <memory>
#include <string>

namespace m8t {

namespace telemetry {
void collect(std::string& out);
}  // namespace telemetry

}  // namespace m8t
#endif  // METALHEART_TELEMETRY_STATE_SERVICE_H