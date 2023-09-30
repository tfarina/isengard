package(default_visibility = ["//visibility:public"])

licenses(["notice"])

cc_library(
    name = "googletest",
    srcs = [
        "gtest-all.cc",
        "gtest_main.cc",
    ],
    hdrs = [
        "gtest.h",
    ],
    includes = [".."],
    # gtest detects pthread, and the linker fails without it.
    linkopts = ["-lpthread"],
)
