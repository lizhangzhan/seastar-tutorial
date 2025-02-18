cc_import(
  name = "hwloc",
  shared_library = select({
      "@platforms//cpu:aarch64": "aarch64-linux-gnu/libhwloc.so",
      "@platforms//cpu:x86_64": "x86_64-linux-gnu/libhwloc.so",
  }),
  visibility = ["//visibility:public"]
)

