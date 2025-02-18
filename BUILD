cc_binary(
  name = "hello_world",
  srcs = ["example/hello_world.cc"],
  deps = [
    "@seastar"
  ]
)

cc_binary(
  name = "echo_demo",
  srcs = ["example/echo_demo.cc"],
  deps = [
    "@seastar"
  ]
)

cc_binary(
  name = "coroutine_demo",
  srcs = ["example/coroutines_demo.cc"],
  defines = ["SEASTAR_COROUTINES_ENABLED"],
  deps = [
    "@seastar"
  ]
)

cc_binary(
  name = "tcp_server",
  srcs = ["example/rpc/tcp_server.cc"],
  deps = [
    "@seastar"
  ]
)

cc_binary(
  name = "tcp_client",
  srcs = ["example/rpc/tcp_client.cc"],
  deps = [
    "@seastar"
  ]
)

cc_library(
    name = "thread_pool",
    hdrs = ["example/thread_pool.h"],
    deps = [
        "@com_google_absl//absl/base",
        "@com_google_absl//absl/synchronization",
    ],
)

cc_binary(
    name = "thread_pool_test",
    srcs = ["example/thread_pool_test.cc"],
    deps = [
        ":thread_pool"
    ],
)
