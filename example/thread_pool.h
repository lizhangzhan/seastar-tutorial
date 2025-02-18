#include <cassert>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <future>
#include <queue>
#include <semaphore>
#include <thread>  // NOLINT(build/c++11)
#include <type_traits>
#include <utility>
#include <vector>

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/blocking_counter.h"
#include "absl/synchronization/mutex.h"

// A simple ThreadPool implementation for tests.
template <int16_t MAX_BUFFER_SIZE = 2>
class ThreadPool {
 public:
  explicit ThreadPool(int num_threads) {
    threads_.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
      threads_.push_back(std::thread(&ThreadPool::WorkLoop, this));
    }
  }

  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  ~ThreadPool() {
    {
      absl::MutexLock l(&mu_);
      for (size_t i = 0; i < threads_.size(); i++) {
        queue_.push(nullptr);  // Shutdown signal.
      }
    }
    for (auto &t : threads_) {
      t.join();
    }
  }

  // Schedule a function to be run on a ThreadPool thread immediately.
  template <typename Func, typename... Args,
            typename ResultType = typename std::result_of<Func(Args...)>::type>
  std::future<ResultType> Schedule(Func &&func, Args &&...args) {
    assert(func != nullptr);
    auto bind_func =
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    auto task = new Task<ResultType>(std::move(bind_func));
    std::future<ResultType> future = task->get_future();
    absl::MutexLock l(&mu_);
    mu_.Await(absl::Condition(this, &ThreadPool::BufferAvailable));
    queue_.push(std::bind(&Task<ResultType>::Run, task));
    return future;
  }

 private:
  template <typename R>
  struct Task {
    std::packaged_task<R()> task;

    template <typename Func>
    explicit Task(Func &&func) : task(std::forward<Func>(func)) {}

    std::future<R> get_future() { return task.get_future(); }

    void Run() {
      task();
      delete this;
    }
  };

  bool TaskAvailable() const ABSL_EXCLUSIVE_LOCKS_REQUIRED(mu_) {
    return !queue_.empty();
  }

  bool BufferAvailable() const ABSL_EXCLUSIVE_LOCKS_REQUIRED(mu_) {
    return queue_.size() < MAX_BUFFER_SIZE;
  }

  void WorkLoop() {
    while (true) {
      std::function<void()> func;
      {
        absl::MutexLock l(&mu_);
        mu_.Await(absl::Condition(this, &ThreadPool::TaskAvailable));
        func = std::move(queue_.front());
        queue_.pop();
      }
      if (func == nullptr) {  // Shutdown signal.
        break;
      }
      func();
    }
  }

  absl::Mutex mu_;
  std::queue<std::function<void()>> queue_ ABSL_GUARDED_BY(mu_);
  std::vector<std::thread> threads_;
};
