load("//third_party/io_uring:workspace.bzl", io_uring = "deps")
load("//third_party/hwloc:workspace.bzl", hwloc = "deps")

def deps():
  io_uring()
  hwloc()
