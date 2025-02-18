#include <signal.h>

#include <iostream>
#include <seastar/core/app-template.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/api.hh>

using namespace std::chrono_literals;

const char* canned_response = "Seastar is the future!\n";

seastar::future<> service_loop() {
  seastar::listen_options lo;
  lo.reuse_address = true;
  return seastar::do_with(
      seastar::listen(seastar::make_ipv4_address({12345}), lo),
      [](auto& listener) {
        return seastar::keep_doing([&listener]() {
          return listener.accept().then([](seastar::accept_result res) {
            auto s = std::move(res.connection);
            auto in = s.input();
            auto out = s.output();
            return seastar::do_with(
                std::move(s), std::move(in), std::move(out),
                [](auto& s, auto& in, auto& out) {
                  return seastar::repeat([&in]() {
                           return in.read().then([](auto buf) {
                             if (buf) {
                               std::cout << std::string(buf.get(), buf.size())
                                         << std::endl;
                               return seastar::make_ready_future<
                                   seastar::stop_iteration>(
                                   seastar::stop_iteration::no);
                             }
                             return seastar::make_ready_future<
                                 seastar::stop_iteration>(
                                 seastar::stop_iteration::yes);
                           });
                         })
                      .then([&out]() {
                        return out.write(canned_response).then([&out] {
                          return out.close();
                        });
                      });
                });
          });
        });
      });
}

int main(int argc, char** argv) {
  seastar::app_template::config cfg;
  cfg.auto_handle_sigint_sigterm = false;
  seastar::app_template app(std::move(cfg));
  app.run(argc, argv, [] {
    return service_loop().handle_exception([](auto ep) {
      std::cerr << "Server failed: " << ep << std::endl;
      return seastar::make_exception_future<>(ep);
    });
  });
}
