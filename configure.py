#!/usr/bin/env python
import sys
import argparse
import subprocess

# CROSS_BINUTILS_VERSION = '2.42'
# CROSS_GCC_VERSION = '14.1.0'

MESON_DEFAULT_CROSS_FILE = "./utils/meson-configs/x86_64-llvm.ini"
MINIMUM_LLVM_MAJOR_VERSION = 18


def get_args():
    parser = argparse.ArgumentParser(
        description="Configure the Meson build environment"
    )
    parser.add_argument(
        "-b",
        "--build",
        choices=["debug", "release"],
        default="debug",
        help="Build type",
    )
    parser.add_argument(
        "meson_args",
        nargs=argparse.REMAINDER,
        help="Additional arguments to pass to Meson",
    )

    return parser.parse_args()


def check_clang_version():
    print("[*] Checking Clang version...")
    compiler_version = None

    try:
        compiler_version = subprocess.check_output(
            "clang --version", shell=True, text=True
        ).split()[2]
    except subprocess.CalledProcessError:
        raise RuntimeError("Error configuring with Meson")

    compiler_major_version = int(compiler_version.split(".")[0])

    if compiler_major_version < MINIMUM_LLVM_MAJOR_VERSION:
        # TODO: Add build option instead of bailing out.
        print("Error: Clang version too old, required >= 18", file=sys.stderr)
        exit(-1)

    print("[*] Valid Clang version")


def meson_configure(args):
    print("[*] Configuring meson...")

    meson_cmd = [
        "meson",
        "setup",
        "--cross-file",
        MESON_DEFAULT_CROSS_FILE,
        "build",
        "--reconfigure",
        "--buildtype",
        args.build,
    ] + args.meson_args[1:]

    try:
        subprocess.check_call(meson_cmd)
    except subprocess.CalledProcessError:
        raise RuntimeError("Error configuring with Meson")


if __name__ == "__main__":
    args = get_args()

    print("BUILD OPTIONS:")
    print("Build type: " + args.build)
    print("Additional Meson arguments: " + str(args.meson_args))
    print("")

    check_clang_version()
    meson_configure(args)
