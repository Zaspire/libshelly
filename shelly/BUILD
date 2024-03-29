package(default_visibility = ["//visibility:public"])

cc_library(
    name = "common",
    hdrs = ["common.hpp"],
)

cc_library(
    name = "algorithm",
    hdrs = ["algorithm.hpp"],
)

cc_library(
    name = "io_redirector",
    hdrs = ["io_redirector.hpp"],
)

cc_library(
    name = "shelly",
    hdrs = ["shelly.hpp"],
)

cc_library(
    name = "avl_tree",
    hdrs = ["avl_tree.hpp"],
    deps = [":common"],
)

cc_library(
    name = "fenwick_tree",
    hdrs = ["fenwick_tree.hpp"],
    deps = [":common"],
)

cc_library(
    name = "string_utils",
    hdrs = ["string_utils.hpp"],
)

cc_library(
    name = "stream",
    hdrs = ["stream.hpp"],
)

cc_library(
    name = "distjoint_set",
    hdrs = ["distjoint_set.hpp"],
)

cc_test(
    name = "algorithm_test",
    srcs = ["algorithm_test.cpp"],
    deps = [
        ":algorithm",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "avl_tree_test",
    srcs = ["avl_tree_test.cpp"],
    deps = [
        ":avl_tree",
        "@com_google_googletest//:gtest_main",
    ],
)

# TODO: split
cc_test(
    name = "common_test",
    srcs = ["common_test.cpp"],
    deps = [
        ":common",
        ":shelly",
        ":fenwick_tree",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "string_utils_test",
    srcs = ["string_utils_test.cpp"],
    deps = [
        ":string_utils",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "stream_test",
    srcs = ["stream_test.cpp"],
    deps = [
        ":stream",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "distjoint_set_test",
    srcs = ["distjoint_set_test.cpp"],
    deps = [
        ":distjoint_set",
        "@com_google_googletest//:gtest_main",
    ],
)
