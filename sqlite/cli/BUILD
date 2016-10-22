cc_library(
    name = "cli",
    srcs = [ "db.c", "user.c" ],
    hdrs = [ "db.h", "user.h" ],
    deps = [ "//third_party/sqlite:sqlite3" ],
)

cc_binary(
    name = "list",
    srcs = [ "list.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)

cc_binary(
    name = "insert",
    srcs = [ "insert.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)

cc_binary(
    name = "update",
    srcs = [ "update.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)

cc_binary(
    name = "delete",
    srcs = [ "delete.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)

cc_binary(
    name = "create",
    srcs = [ "create.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)

cc_binary(
    name = "all",
    srcs = [ "all.c" ],
    deps = [ ":cli" ],
    copts = ["-std=c99"],
    linkopts = [ "-pthread", "-ldl" ],
)
