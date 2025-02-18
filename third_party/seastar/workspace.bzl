
def deps():
    native.new_local_repository(
        name = "seastar",
        path = "/usr/local/lib",
        build_file = "//third_party/seastar:seastar.BUILD"
    )
