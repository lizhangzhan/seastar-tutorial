load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def deps():
    ABSL_COMMIT = "6f9d96a1f41439ac172ee2ef7ccd8edf0e5d068c"
    ABSL_SHA256 = "62c27e7a633e965a2f40ff16b487c3b778eae440bab64cad83b34ef1cbe3aa93"

    http_archive(
        name = "com_google_absl",
        sha256 = ABSL_SHA256,
        strip_prefix = "abseil-cpp-{commit}".format(commit = ABSL_COMMIT),
        urls = [
            "https://github.com/abseil/abseil-cpp/archive/{commit}.tar.gz".format(commit = ABSL_COMMIT),
        ],
    )
