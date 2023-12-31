#include <unistd.h>

#include <cstdio>
#include <format>
#include <iostream>
#include <unordered_map>

#include "redis/redis_adapter.h"

int main(int argc, char *argv[]) {
  pid_t pid = getpid();
  std::cout << std::format("[{}]:... start\n", pid);

  // redis::sampleDistributedLock();

  std::cout << std::format("[{}]:... end\n", pid);
  return 0;
}
