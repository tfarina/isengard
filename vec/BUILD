cc_library(
    name = "os",
    srcs = ["os_unix.c"],
    hdrs = ["os.h"],
)

cc_library(
    name = "vec",
    srcs = ["vec.c"],
    hdrs = ["vec.h"],
    deps = [":os"],
)

cc_test(
    name = "vec_test",
    srcs = ["vec_test.c"],
    deps = [":vec"],
)
