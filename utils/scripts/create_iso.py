#!/usr/bin/env python
import os
import sys
import shutil
import subprocess


def get_b2sum(cfg_path):
    try:
        result = subprocess.check_output(["b2sum", cfg_path], text=True)
        return result.split()[0]
    except subprocess.CalledProcessError:
        exit(-1)


def enroll_config(iso_dir):
    limine_utility = iso_dir + os.sep + "limine"
    limine_cfg = iso_dir + os.sep + "limine.cfg"
    limine_bios_sys = iso_dir + os.sep + "limine-bios.sys"

    command = [
        limine_utility,
        "enroll-config",
        "--quiet",
        limine_bios_sys,
        get_b2sum(limine_cfg),
    ]

    try:
        subprocess.check_call(command)
    except subprocess.CalledProcessError:
        exit(-1)

    # Remove copied Limine utility
    os.remove(limine_utility)


def create_iso_dir(limine_dir, kernel, iso_dir):
    iso_dir_efi = iso_dir + os.sep + 'EFI' + os.sep + 'BOOT'
    limine_boot_efi = iso_dir + os.sep + 'BOOTX64.EFI'

    if os.path.exists(iso_dir):
        shutil.rmtree(iso_dir)

    shutil.copytree(limine_dir, iso_dir)
    shutil.copy(kernel, iso_dir)

    os.makedirs(iso_dir_efi)
    shutil.move(limine_boot_efi, iso_dir_efi)


def build_iso(iso_dir, iso_file):
    command = [
        "xorriso",
        "-as",
        "mkisofs",
        "-b",
        "limine-bios-cd.bin",
        "-no-emul-boot",
        "-boot-load-size",
        "4",
        "-boot-info-table",
        "--efi-boot",
        "limine-uefi-cd.bin",
        "-efi-boot-part",
        "--efi-boot-image",
        "--protective-msdos-label",
        iso_dir,
        "-o",
        iso_file,
    ]

    try:
        subprocess.check_call(command)
    except subprocess.CalledProcessError:
        exit(-1)


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print(
            "usage: ./"
            + sys.argv[0]
            + " <limine_dir> <kernel> <isodir> <iso>",
            file=sys.stderr,
        )
        exit(-1)

    kernel = sys.argv[1]
    limine_dir = sys.argv[2]
    iso_dir = sys.argv[3]
    iso_file = sys.argv[4]

    create_iso_dir(limine_dir, kernel, iso_dir)
    enroll_config(iso_dir)
    build_iso(iso_dir, iso_file)
