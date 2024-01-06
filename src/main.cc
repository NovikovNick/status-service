#include <sys/resource.h>
#include <unistd.h>

#include <cassert>
#include <chrono>
#include <cstdio>
#include <format>
#include <iostream>

int main(int argc, char* argv[]) {
  pid_t pid = getpid();

  std::cout << std::format("[{}]:... start\n", pid);

  std::cout << std::format("[{}]:... end\n", pid);
  return 0;
}
