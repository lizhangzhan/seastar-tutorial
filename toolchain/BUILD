load(":cc_toolchain_config.bzl", "cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(name = "empty")

cc_toolchain_config(
  name = "aarch64_toolchain_config",
  extra_include_directories = [
    "/usr/lib/llvm-18/lib/clang/18/include/",
    "/usr/include/aarch64-linux-gnu/",
  ]
)

cc_toolchain_config(
  name = "k8_toolchain_config",
  extra_include_directories = [
    "/usr/lib/llvm-18/lib/clang/18/include/",
    "/usr/include/x86_64-linux-gnu/",
  ]
)

cc_toolchain(
  name = "aarch64_toolchain",
  toolchain_identifier = "aarch64-toolchain",
  toolchain_config = ":aarch64_toolchain_config",
  all_files = ":empty",
  compiler_files = ":empty",
  dwp_files = ":empty",
  linker_files = ":empty",
  objcopy_files = ":empty",
  strip_files = ":empty",
  supports_param_files = 0,
)

cc_toolchain(
  name = "k8_toolchain",
  toolchain_identifier = "k8-toolchain",
  toolchain_config = ":k8_toolchain_config",
  all_files = ":empty",
  compiler_files = ":empty",
  dwp_files = ":empty",
  linker_files = ":empty",
  objcopy_files = ":empty",
  strip_files = ":empty",
  supports_param_files = 0,
)

cc_toolchain_suite(
  name = "clang_suite",
  toolchains = {
    "aarch64": ":aarch64_toolchain",
    "k8": ":k8_toolchain",
  }
)
