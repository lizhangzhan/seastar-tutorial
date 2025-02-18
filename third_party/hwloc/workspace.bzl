
def deps():
    native.new_local_repository(
        name = "hwloc",
        path = "/usr/lib",
        build_file = "//third_party/hwloc:hwloc.BUILD"
    )
