workspace(name = "seastar-tutorial")

load("//:workspace.bzl", setup_tools = "deps")
setup_tools()

load("@rules_foreign_cc//foreign_cc:repositories.bzl", rules_foreign_cc_dependencies = "rules_foreign_cc_dependencies")
rules_foreign_cc_dependencies()

load("//third_party/absl:workspace.bzl", absl= "deps")
absl()

load("//third_party/seastar:seastar_deps.bzl", seastar = "deps")
seastar()

load("//third_party/seastar:workspace.bzl", seastar= "deps")
seastar()

load("//third_party/jsoncpp:workspace.bzl", jsoncpp = "deps")
jsoncpp()
