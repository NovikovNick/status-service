#ifndef METALHEART_SERVER_STATE_SERVICE_H
#define METALHEART_SERVER_STATE_SERVICE_H

namespace m8t {
class StateService final {
  StateService() = default;

 public:
  StateService(const StateService&)            = delete;
  StateService& operator=(const StateService&) = delete;

  [[nodiscard]] static StateService& instance() {
    static StateService inst;
    return inst;
  }

  int uptimeInSeconds() const;
};

}  // namespace m8t
#endif  // METALHEART_SERVER_STATE_SERVICE_H