cc_library(
    name = "umgr",
    srcs = [
        "db.c",
        "ab.c",
    ],
    hdrs = [
        "db.h",
        "ab.h",
    ],
    deps = ["//third_party/sqlite:sqlite3"],
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
    name = "change",
    srcs = ["change.c"],
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
    name = "showrec",
    srcs = ["showrec.c"],
    copts = ["-std=c99"],
    linkopts = [
        "-pthread",
        "-ldl",
    ],
    deps = [":umgr"],
)
