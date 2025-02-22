#include <iostream>
#include <seastar/core/app-template.hh>
#include <seastar/core/future.hh>
#include <seastar/core/iostream.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/api.hh>
#include <seastar/util/log.hh>
#include <stdexcept>

using namespace std::chrono_literals;
using namespace seastar;

seastar::future<> read_message(seastar::input_stream<char>&& in) {
  std::cout << "start read response:" << std::endl;
  return seastar::repeat([in = std::move(in)]() mutable {
    return in.read().then([](auto buf) {
      if (buf) {
        std::cout << std::string(buf.get(), buf.size()) << std::endl;
        return seastar::make_ready_future<seastar::stop_iteration>(
            seastar::stop_iteration::no);
      }
      return seastar::make_ready_future<seastar::stop_iteration>(
          seastar::stop_iteration::yes);
    });
  });
}

seastar::future<> send_request_with_attachment(
    seastar::output_stream<char>&& out, std::string&& main_message,
    std::string&& attachment) {
  // Send the header (main message size + attachment size)
  return seastar::do_with(
      std::move(out), std::move(main_message), std::move(attachment),
      [](auto& out, auto& main_message, auto& attachment) {
        auto main_message_size = main_message.size();
        return out
            .write(reinterpret_cast<char*>(&main_message_size),
                   sizeof(uint32_t))
            .then([&out, &main_message] {
              // Send the main message
              return out.write(main_message);
            })
            .then([&out, &attachment] {
              auto attachment_size = attachment.size();
              // Send the attachment size
              return out.write(reinterpret_cast<char*>(&attachment_size),
                               sizeof(uint32_t));
            })
            .then([&out, &attachment] {
              // Send the attachment
              return out.write(attachment);
            })
            .then([&out] { return out.close(); });
      });
}

auto tcp_client() {
  auto server_addr = seastar::make_ipv4_address({"127.0.0.1", 12345});
  auto local_addr =
      seastar::socket_address(::sockaddr_in{AF_INET, INADDR_ANY, {0}});

  return seastar::connect(std::move(server_addr), std::move(local_addr),
                          seastar::transport::TCP)
      .then([](auto connected_socket) {
        std::cout << "successfully connection" << std::endl;
        auto out = connected_socket.output();
        auto in = connected_socket.input();

        std::string main_message = "Hello, server!";
        std::string attachment = "This is a large binary attachment";

        return send_request_with_attachment(std::move(out),
                                            std::move(main_message),
                                            std::move(attachment))
            .then([in = std::move(in)]() mutable {
              return read_message(std::move(in));
            });
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
