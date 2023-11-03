#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "redis_adapter.h"

int main(int argc, char *argv[]) {
  printf("[main]:... start\n");

  redis::sampleDistributedLock();
  
  printf("[main]:... end\n");
  return 0;
}
