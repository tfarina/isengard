cc_library(
    name = "xalloc",
    srcs = ["xalloc.c"],
    hdrs = ["xalloc.h"],
)

cc_library(
    name = "string_list",
    srcs = ["string_list.c"],
    hdrs = ["string_list.h"],
    deps = [":xalloc"],
)

cc_test(
    name = "string_list_test",
    srcs = ["string_list_test.c"],
    deps = [":string_list"],
)
