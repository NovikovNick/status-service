#ifndef METALHEART_REDIS_DISTRIBUTED_LOCK_H
#define METALHEART_REDIS_DISTRIBUTED_LOCK_H

#include <hiredis.h>
#include <stdarg.h>
#include <stdio.h>
#include <strings.h>

#include <utility>

namespace redis {

void debug(const char *format, ...) {
#ifdef DEBUG
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
#endif
}

class DistributedLock {
  int           key_;
  redisContext *ctx_;
  bool          locked_;

 public:
  DistributedLock(redisContext *ctx, int key)
      : key_(key),         //
        ctx_(ctx),         //
        locked_(false){};  //

  DistributedLock(const DistributedLock &)            = delete;
  DistributedLock &operator=(const DistributedLock &) = delete;

  DistributedLock(DistributedLock &&other) noexcept { *this = std::move(other); };

  DistributedLock &operator=(DistributedLock &&other) noexcept {
    if (this == &other) return *this;
    this->key_    = std::exchange(other.key_, 0);
    this->ctx_    = std::exchange(other.ctx_, nullptr);
    this->locked_ = std::exchange(other.locked_, false);
    return *this;
  };

  ~DistributedLock() {
    try {
      if (locked_) {
        redisCommand(ctx_, "DEL lock:%d", key_);
        debug("[redis]:... user_id = %d unlocked \n", key_);
      }
    } catch (...) {
    }
  };

  bool try_lock() {
    if (!ctx_) return false;

    redisReply *reply =
        static_cast<redisReply *>(redisCommand(ctx_, "SET lock:%d 0 NX EX 5", key_));

    if (reply && reply->len == 2 && strcasecmp(reply->str, "ok") == 0) {
      debug("[redis]:... user_id = %d locked\n", key_);

      locked_ = true;
      return true;

    } else {
      debug("[redis]:... user_id = %d unable to lock\n", key_);
      return false;
    };
  };
};

}  // namespace redis

#endif  // METALHEART_REDIS_DISTRIBUTED_LOCK_H