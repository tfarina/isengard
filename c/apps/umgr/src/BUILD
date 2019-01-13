cc_library(
    name = "umgr",
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
    deps = [":umgr"],
)

cc_binary(
    name = "add",
    srcs = ["add.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)

cc_binary(
    name = "update",
    srcs = ["update.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)

cc_binary(
    name = "delete",
    srcs = ["delete.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)

cc_binary(
    name = "init",
    srcs = ["init.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)

cc_binary(
    name = "all",
    srcs = ["all.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)
