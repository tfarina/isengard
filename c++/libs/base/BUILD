package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    srcs = [
        "file_util.cc",
        "path.cc",
    ],
    hdrs = [
        "file_util.h",
        "path.h",
    ],
)

cc_test(
    name = "base_unittests",
    size = "small",
    srcs = ["path_test.cc"],
    deps = ["//third_party/gtest"],
)
