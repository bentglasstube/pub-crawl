package(default_visibility = ["//visibility:public"])

load("@bazel_tools//tools/build_defs/pkg:pkg.bzl", "pkg_tar")
load("@mxebzl//tools/windows:rules.bzl", "pkg_winzip")

config_setting(
    name = "windows",
    values = {
        "crosstool_top": "@mxebzl//tools/windows:toolchain",
    }
)

cc_binary(
    name = "ld40",
    data = ["//content"],
    linkopts = select({
        ":windows": ["-mwindows", "-lSDL2main" ],
        "//conditions:default": [],
    }) + [
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

pkg_winzip(
    name = "ld40-windows",
    files = [
        ":ld40",
        "//content",
    ]
)

pkg_tar(
    name = "ld40-linux",
    extension = "tgz",
    srcs = [
        ":ld40",
        "//content",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "map_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "map_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:screen",
        "@libgam//:text",
        "map",
        "player",
    ],
)

cc_library(
    name = "map",
    srcs = [ "map.cc" ],
    hdrs = [ "map.h" ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:util",
    ],
)

cc_library(
    name = "player",
    srcs = [ "player.cc" ],
    hdrs = [ "player.h" ],
    deps = [
        "@libgam//:graphics",
        "map",
    ],
)