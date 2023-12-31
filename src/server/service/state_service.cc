#include "state_service.h"

#include <chrono>

using Clock = std::chrono::steady_clock;

namespace {
const auto started_at_ = Clock::now();
}  // namespace

namespace m8t {
int StateService::uptimeInSeconds() const {
  using namespace std::chrono;
  return duration_cast<seconds>(Clock::now() - started_at_).count();
};
};  // namespace m8t
