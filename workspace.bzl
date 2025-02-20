
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def deps():
  maybe(
      http_archive,
      name = "bazel_skylib",
      sha256 = "9f38886a40548c6e96c106b752f242130ee11aaa068a56ba7e56f4511f33e4f2",
      urls = [
          "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.6.1/bazel-skylib-1.6.1.tar.gz",
          "https://github.com/bazelbuild/bazel-skylib/releases/download/1.6.1/bazel-skylib-1.6.1.tar.gz",
      ],
  )

  release_tag = "0.9.0"
  http_archive(
      name = "rules_foreign_cc",
      sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
      strip_prefix = "rules_foreign_cc-{}".format(release_tag),
      url = "https://github.com/bazel-contrib/rules_foreign_cc/archive/{}.tar.gz".format(release_tag),
  )
