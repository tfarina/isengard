package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    srcs = [
        "directory.cc",
        "file_util.cc",
        "path.cc",
    ],
    hdrs = [
        "arraysize.h",
        "directory.h",
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
