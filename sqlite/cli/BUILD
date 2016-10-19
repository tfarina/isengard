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
