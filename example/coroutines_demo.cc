#include <iostream>

#include <seastar/util/std-compat.hh>

#ifndef SEASTAR_COROUTINES_ENABLED

int main(int argc, char **argv) {
  std::cout << "coroutines not available\n";
  return 0;
}

#else

#include <seastar/core/app-template.hh>
#include <seastar/core/coroutine.hh>
#include <seastar/core/fstream.hh>
#include <seastar/core/loop.hh>
#include <seastar/core/seastar.hh>
#include <seastar/core/sleep.hh>
#include <seastar/core/sstring.hh>
#include <seastar/coroutine/parallel_for_each.hh>

int main(int argc, char **argv) {
  seastar::app_template app;
  app.run(argc, argv, []() -> seastar::future<> {
    std::cout << "this is a completely useless program\nplease stand by...\n";
    auto f = seastar::coroutine::parallel_for_each(
        std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        [](int i) -> seastar::future<> {
          co_await seastar::sleep(std::chrono::seconds(i));
          std::cout << i << "\n";
        });

    auto file = co_await seastar::open_file_dma("useless_file.txt",
                                                seastar::open_flags::create |
                                                    seastar::open_flags::wo);
    auto out = co_await seastar::make_file_output_stream(file);
    seastar::sstring str = "nothing to see here, move along now\n";
    co_await out.write(str);
    co_await out.flush();
    co_await out.close();

    bool all_exist = true;
    std::vector<seastar::sstring> filenames = {"useless_file.txt",
                                               "non_existing"};
    co_await seastar::coroutine::parallel_for_each(
        filenames,
        [&all_exist](const seastar::sstring &name) -> seastar::future<> {
          all_exist &= co_await seastar::file_exists(name);
        });
    std::cout << (all_exist ? "" : "not ") << "all files exist" << std::endl;

    co_await std::move(f);
    std::cout << "done\n";
  });
}

#endif
