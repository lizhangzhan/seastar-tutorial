cc_import(
  name = "io_uring",
  shared_library = select({
      "@platforms//cpu:aarch64": "aarch64-linux-gnu/liburing.so",
      "@platforms//cpu:x86_64": "x86_64-linux-gnu/liburing.so",
  }),
  visibility = ["//visibility:public"]
)

