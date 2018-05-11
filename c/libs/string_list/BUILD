cc_library(
    name = "xalloc",
    srcs = ["xalloc.c"],
    hdrs = ["xalloc.h"],
)

cc_library(
    name = "string_array",
    srcs = ["string_array.c"],
    hdrs = ["string_array.h"],
    deps = [":xalloc"],
)

cc_test(
    name = "string_array_test",
    srcs = ["string_array_test.c"],
    deps = [":string_array"],
)
