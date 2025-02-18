#include <iostream>
#include <seastar/core/app-template.hh>
#include <seastar/core/future.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/api.hh>
#include <seastar/util/log.hh>
#include <stdexcept>

using namespace std::chrono_literals;
using namespace seastar;

seastar::future<> handle_response(auto s) {
  auto out = s.output();
  auto in = s.input();
  return do_with(
      std::move(s), std::move(out), std::move(in),
      [](auto& s, auto& out, auto& in) {
        return out.write("hello server")
            .then([&out]() { return out.close(); })
            .then([&in] {
              return seastar::repeat([&in]() {
                return in.read().then([](auto buf) {
                  if (buf) {
                    std::cout << std::string(buf.get(), buf.size())
                              << std::endl;
                    return seastar::make_ready_future<seastar::stop_iteration>(
                        seastar::stop_iteration::no);
                  }
                  return seastar::make_ready_future<seastar::stop_iteration>(
                      seastar::stop_iteration::yes);
                });
              });
            });
      });
}

auto tcp_client() {
  // Server address (replace with your server's IP and port)
  auto server_addr = seastar::make_ipv4_address({"127.0.0.1", 12345});
  auto local_addr =
      seastar::socket_address(::sockaddr_in{AF_INET, INADDR_ANY, {0}});
  return seastar::connect(std::move(server_addr), std::move(local_addr),
                          seastar::transport::TCP)
      .then([](auto connected_socket) {
        std::cout << "successfully connection" << std::endl;
        return handle_response(std::move(connected_socket));
      });
}

int main(int argc, char** argv) {
  seastar::app_template app;
  app.run(argc, argv, [] {
    return tcp_client().handle_exception([](auto ep) {
      std::cerr << "Client error: " << ep << std::endl;
      return seastar::make_exception_future<>(ep);
    });
  });
  return 0;
}
