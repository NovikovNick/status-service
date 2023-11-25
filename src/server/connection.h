#ifndef METALHEART_TCP_CONNECTION_H
#define METALHEART_TCP_CONNECTION_H

namespace tcp {

class Connection {
  char buf_[1500];
  int endpoint_;

 public:
  Connection(int endpoint_);

  Connection(Connection &&) noexcept;

  Connection &operator=(Connection &&) noexcept;

  ~Connection();

  void handle();
};

}  // namespace tcp
#endif  // METALHEART_TCP_CONNECTION_H