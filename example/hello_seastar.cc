#include <iostream>
#include <seastar/core/app-template.hh>
#include <seastar/core/future.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/util/log.hh>
#include <stdexcept>

using namespace std::chrono_literals;
using namespace seastar;
logger applog("app");

seastar::future<> f() {
  std::cout << "sleeping..." << std::flush;
  (void)seastar::sleep(200ms).then(
      []() { std::cout << ",200ms" << std::flush; });
  (void)seastar::sleep(100ms).then(
      []() { std::cout << ",100ms" << std::flush; });
  return seastar::sleep(1s).then([]() { std::cout << ",Done" << std::endl; });
}

int main(int argc, char **argv) {
  seastar::app_template::config cfg;
  cfg.auto_handle_sigint_sigterm = false;
  seastar::app_template app(std::move(cfg));
  (void)app.run(argc, argv, f);
  return 0;
}
