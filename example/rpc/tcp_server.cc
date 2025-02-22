#include <seastar/core/app-template.hh>
#include <seastar/core/future-util.hh>
#include <seastar/core/future.hh>
#include <seastar/core/iostream.hh>
#include <seastar/core/reactor.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/net/api.hh>
#include <seastar/util/log.hh>

seastar::logger logger("tester");

seastar::future<> handle_connection(seastar::input_stream<char>&& in,
                                    seastar::output_stream<char>&& out) {
  return seastar::do_with(
      std::move(in), std::move(out), [](auto& in, auto& out) {
        return in.read_exactly(sizeof(uint32_t))
            .then([&in, &out](auto header_buf) {
              uint32_t main_message_size;
              std::memcpy(&main_message_size, header_buf.get(),
                          sizeof(uint32_t));

              return in.read_exactly(main_message_size)
                  .then([&in, &out](auto main_message_buf) {
                    // Deserialize the main message
                    std::string main_message(main_message_buf.get(),
                                             main_message_buf.size());

                    // Read the attachment size
                    return in.read_exactly(sizeof(uint32_t))
                        .then([&in, &out](auto attachment_size_buf) {
                          uint32_t attachment_size;
                          std::memcpy(&attachment_size,
                                      attachment_size_buf.get(),
                                      sizeof(uint32_t));

                          // Read the attachment
                          return in.read_exactly(attachment_size)
                              .then([&out](auto attachment_buf) {
                                // Process the attachment (e.g., save to disk)
                                std::string attachment(attachment_buf.get(),
                                                       attachment_buf.size());
                                logger.info("Received attachment of size: {}",
                                            attachment.size());

                                // Send a response
                                return out.write("OK").then(
                                    [&out] { return out.close(); });
                              });
                        });
                  });
            });
      });
}

seastar::future<> service_loop() {
  seastar::listen_options lo;
  lo.reuse_address = true;
  return seastar::do_with(
      seastar::listen(seastar::make_ipv4_address({12345})), [](auto& listener) {
        return seastar::keep_doing([&listener] {
          return listener.accept().then([](seastar::accept_result res) {
            auto in = res.connection.input();
            auto out = res.connection.output();
            return handle_connection(std::move(in), std::move(out));
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
