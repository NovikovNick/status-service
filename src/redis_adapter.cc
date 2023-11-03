#include "redis_adapter.h"

#include <hiredis.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "distributed_lock.h"

void redis::sampleDistributedLock() {
  auto redis_host = "127.0.0.1";
  auto redis_port = 6379;

  redisContext *ctx = redisConnect(redis_host, redis_port);
  if (ctx->err != 0) {
    printf("[redis]:... Redis is unavailable.\n");
    return;
  }

  printf("[redis]:... Redis is available.\n");

  auto lock1 = DistributedLock(ctx, 0);
  auto lock2 = DistributedLock(ctx, 0);
  auto lock3 = std::move(lock2);

  lock1.try_lock();
  //lock2.try_lock();
  lock3.try_lock();
}
