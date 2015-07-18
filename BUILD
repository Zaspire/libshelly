genrule(
    name = "genrule2",
    outs = ["empty.cpp"],
    cmd = "touch $@",
)

cc_library(
    name = "shelly",
    srcs = ["empty.cpp"],
    includes = ["include"],
    hdrs = glob(["include/*.hpp"]),
    visibility = ["//visibility:public"],
)