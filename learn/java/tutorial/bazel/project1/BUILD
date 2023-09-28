java_binary(
    name = "my-runner",
    srcs = glob(["**/*.java"]),
    main_class = "com.example.ProjectRunner",
)

java_library(
    name = "greeter",
    srcs = ["src/main/java/com/example/Greeting.java"],
    visibility = ["//src/main/java/com/example/cmdline:__pkg__"],
)

java_binary(
    name = "my-other-runner",
    srcs = ["src/main/java/com/example/ProjectRunner.java"],
    main_class = "com.example.ProjectRunner",
    deps = [":greeter"],
)
