#include <hiredis.h>

#include <vector>
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char *argv[]) {
  printf("[main]:... start\n");

  std::vector<int> arr;
  arr.reserve(1000000);

  redisContext *ctx = redisConnect("127.0.0.1", 6379);
  if (ctx->err == 0) {
    printf("[main]:... Redis is available.\n");
  } else {
    printf("[main]:... Redis is unavailable.\n");
    return 1;
  }

  redisReply *reply;

  using namespace std::chrono_literals;
  while (true) {
    reply = static_cast<redisReply *>(redisCommand(ctx, "PING"));

    if (reply) {
      printf("[main]:... PING: %s\n", reply->str);
    } else {
      printf("[main]:... PING: NULL\n");
      return 1;
    }
    std::this_thread::sleep_for(1s);
  }

  printf("[main]:... end\n");
  return 0;
}
