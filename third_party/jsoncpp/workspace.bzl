load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def deps():
  http_archive(
    name = "com_github_jsoncpp",
    build_file = "//third_party/jsoncpp:jsoncpp.BUILD",
    strip_prefix = "jsoncpp-1.9.5",
    sha256 = "f409856e5920c18d0c2fb85276e24ee607d2a09b5e7d5f0a371368903c275da2",
    url = "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.5.tar.gz",
  )
