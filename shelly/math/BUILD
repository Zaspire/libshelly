cc_library(
    name = "big_int",
    hdrs = ["big_int.hpp"],
    deps = ["//shelly:common"],
)

cc_library(
    name = "fraction",
    hdrs = ["fraction.hpp"],
    deps = [":math"],
)

cc_library(
    name = "geometry",
    hdrs = ["geometry.hpp"],
)

cc_library(
    name = "math",
    hdrs = ["math.hpp"],
    deps = ["//shelly:common"],
)

cc_library(
    name = "matrix",
    hdrs = ["matrix.hpp"],
)

cc_test(
    name = "big_int_test",
    srcs = ["big_int_test.cpp"],
    deps = [
        ":big_int",
	"@com_google_googletest//:gtest_main"
    ],
)

cc_test(
    name = "fraction_test",
    srcs = ["fraction_test.cpp"],
    deps = [
        ":fraction",
	"@com_google_googletest//:gtest_main"
    ],
)

cc_test(
    name = "geometry_test",
    srcs = ["geometry_test.cpp"],
    deps = [
        ":geometry",
	"@com_google_googletest//:gtest_main"
    ],
)

cc_test(
    name = "math_test",
    srcs = ["math_test.cpp"],
    deps = [
        ":math",
	"@com_google_googletest//:gtest_main"
    ],
)

cc_test(
    name = "matrix_test",
    srcs = ["matrix_test.cpp"],
    deps = [
        ":matrix",
	"@com_google_googletest//:gtest_main"
    ],
)
