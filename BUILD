package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")

cc_binary(
    name = "pub-crawl",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
        "-static-libstdc++",
        "-static-libgcc",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

cc_library(
    name = "game_state",
    srcs = [ "game_state.cc" ],
    hdrs = [ "game_state.h" ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:text",
        "map",
        "player",
    ],
)

cc_library(
    name = "building",
    srcs = [ "building.cc" ],
    hdrs = [ "building.h" ],
    deps = [
        "@libgam//:graphics",
    ],
)

cc_library(
    name = "map",
    srcs = [ "map.cc" ],
    hdrs = [ "map.h" ],
    deps = [
        "@libgam//:graphics",
        "building",
    ],
)

cc_library(
    name = "message_box",
    srcs = [ "message_box.cc" ],
    hdrs = [ "message_box.h" ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
        "@libgam//:text",
    ],
)

cc_library(
    name = "player",
    srcs = [ "player.cc" ],
    hdrs = [ "player.h" ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:util",
        "map",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "bar_screen.cc",
        "game_over_screen.cc",
        "home_screen.cc",
        "map_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "bar_screen.h",
        "game_over_screen.h",
        "home_screen.h",
        "map_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:text",
        "@libgam//:util",
        "map",
        "message_box",
        "game_state",
    ],
)
