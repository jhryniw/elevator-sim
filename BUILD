cc_library(
    name = "comms",
    srcs = ["packet.cpp", "pipe.cpp"],
    hdrs = ["packet.h", "pipe.h"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [
        "//sim",
        "//:comms"
    ]
)
