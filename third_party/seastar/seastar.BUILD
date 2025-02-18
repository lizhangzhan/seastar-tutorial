cc_import(
  name = "internal_seastar",
  static_library = "libseastar.a",
)

cc_library(
  name = "seastar",
  copts = [
    "-I/usr/local/include",
    "-I/usr/include/p11-kit-1",
    "-U_FORTIFY_SOURCE -Wno-maybe-uninitialized -march=native -Wno-error=unused-result",
  ],
  linkopts = select({
      "@platforms//cpu:aarch64": ["-L/usr/lib/aarch64-linux-gnu/"],
      "//conditions:default": ["-L/usr/lib/x86_64-linux-gnu/"],
    }) + [
    "-L/usr/local/lib",
    "-lboost_program_options",
    "-lboost_thread",
    "-lcares",
    "-lcryptopp",
    "-lfmt -lsctp -lnuma",
    "-ldl",
    "-latomic -lgnutls -lgmp -latomic -lhogweed -lgmp -lnettle -ltasn1 -lidn2 -lunistring -lp11-kit -lm -ludev -lpthread -lyaml-cpp -llz4",
  ],
  deps = [
    ":internal_seastar",
    "@io_uring",
    "@hwloc",
  ],
  visibility = ["//visibility:public"]
)

