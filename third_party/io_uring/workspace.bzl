
def deps():
    native.new_local_repository(
        name = "io_uring",
        path = "/usr/lib",
        build_file = "//third_party/io_uring:io_uring.BUILD"
    )
