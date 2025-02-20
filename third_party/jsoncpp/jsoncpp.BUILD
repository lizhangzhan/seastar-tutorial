load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

cmake(
    name = "jsoncpp",
    cache_entries = {
        "CMAKE_BUILD_TYPE": "Release",
    },
    lib_source = "//:all_srcs",
    out_shared_libs = ["libjsoncpp.so"],
    visibility = ["//visibility:public"],
)
