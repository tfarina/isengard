cc_library(
    name = "os",
    srcs = ["os_unix.c"],
    hdrs = ["os.h"],
)

cc_library(
    name = "string_list",
    srcs = ["string_list.c"],
    hdrs = ["string_list.h"],
    deps = [":os"],
)

cc_test(
    name = "string_list_test",
    srcs = ["string_list_test.c"],
    deps = [":string_list"],
)
