#include "example/thread_pool.h"
#include <iostream>

int main() {
  ThreadPool thread_pool{2};
  auto future =
      thread_pool.Schedule([](std::string &&msg) { return msg; }, "hello");
  std::cout << future.get() << std::endl;
  return 0;
}
