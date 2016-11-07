cc_library(
    name = "cli",
    srcs = [
        "db.c",
        "user.c",
    ],
    hdrs = [
        "db.h",
        "user.h",
    ],
    deps = ["//third_party/sqlite:sqlite3"],
)

cc_binary(
    name = "list",
    srcs = ["list.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)

cc_binary(
    name = "insert",
    srcs = ["insert.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)

cc_binary(
    name = "update",
    srcs = ["update.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)

cc_binary(
    name = "delete",
    srcs = ["delete.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)

cc_binary(
    name = "create",
    srcs = ["create.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)

cc_binary(
    name = "all",
    srcs = ["all.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":cli"],
)
