genrule(
    name = "genrule2",
    outs = ["empty.cpp"],
    cmd = "touch $@",
)

cc_library(
    name = "shelly",
    srcs = ["empty.cpp"],
    includes = ["."],
    hdrs = glob(["shelly/*.hpp"]),
    visibility = ["//visibility:public"],
)