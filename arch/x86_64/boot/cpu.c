/*
 * Copyright © 2024 Guillot Tony <tony.guillot@protonmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <phoenix/kernel.h>
#include <phoenix/mem.h>
#include <phoenix/cpu.h>
#include <cpuid.h>

static const char* cpuid_vendors[] = {
    /* Reals CPUs */
    [0] = "AuthenticAMD",
    [1] = "AMDisbetter!",
    [2] = "GenuineIntel",
    [3] = "GenuineIotel",
    [4] = "VIA VIA VIA",
    [5] = "GenuineTMx86",
    [6] = "TransmetaCPU",
    [7] = "CyrixInstead",
    [8] = "CentaurHauls",
    [9] = "NexGenDriven",
    [10] = "UMC UMC UMC ",
    [11] = "SiS SiS SiS ",
    [12] = "Geode by NSC",
    [13] = "RiseRiseRise",
    [14] = "Vortex86 SoC",
    [15] = "MiSTer AO486",
    [16] = "GenuineAO486",
    [17] = "  Shanghai  ",
    [18] = "HygonGenuine",
    [19] = "Genuine  RDC",
    [20] = "E2K MACHINE ",
    /* Hypervisors CPUs */
    [21] = "TCGTCGTCGTCG",
    [22] = " KVMKVMKVM  ",
    [23] = "VMwareVMware",
    [24] = "VBoxVBoxVBox",
    [25] = "XenVMMXenVMM",
    [26] = "Microsoft Hv",
    [27] = "MicrosoftXTA",
    [28] = "VirtualApple",
    [29] = " prl hyperv ",
    [30] = " lrpepyh vr ",
    [31] = "bhyve bhyve ",
    [32] = " QNXQVMBSQG ",
};

u8 cpuid_vendors_count = sizeof(cpuid_vendors) / sizeof(cpuid_vendors[0]);

enum cpu_brand : u8 {
    /* Reals CPUs */
    AMD,
    INTEL,
    VIA,
    TRANSMETA,
    CYRIX,
    CENTAUR,
    NEXGEN,
    UMC,
    SIS,
    NSC,
    RISE,
    VORTEX,
    AO486,
    ZHAOXIN,
    HYGON,
    RDC,
    ELBRUS,
    /* Hypervisors CPUs */
    QEMU,
    KVM,
    VMWARE,
    VIRTUALBOX,
    XEN,
    HYPERV,
    PARALLELS,
    BHYVE,
    QNX,
    MICROSOFT_XTA,
    APPLE_ROSETTA_2,
    /* What is this? */
    UNKNOWN
};

static u8 cpu_get_brand_id(void) {
    u32 eax;
    u32 regs[3];

    __cpuid(0, eax, regs[0], regs[2], regs[1]);

    char cpu_str[13];
    memcpy(cpu_str, regs, sizeof(regs));
    cpu_str[12] = '\0';

    for (int i = 0; i < cpuid_vendors_count; i++) {
        if (!strcmp(cpu_str, cpuid_vendors[i])) {
            return i;
        }
    }

    return cpuid_vendors_count;
}

static enum cpu_brand cpu_get_brand(void) {
    switch (cpu_get_brand_id()) {
    case 0:
    case 1:
        return AMD;

    case 2:
    case 3:
        return INTEL;

    case 4:
        return VIA;

    case 5:
    case 6:
        return TRANSMETA;

    case 7:
        return CYRIX;

    case 8:
        return CENTAUR;

    case 9:
        return NEXGEN;

    case 10:
        return UMC;

    case 11:
        return SIS;

    case 12:
        return NSC;

    case 13:
        return RISE;

    case 14:
        return VORTEX;

    case 15:
    case 16:
        return AO486;

    case 17:
        return ZHAOXIN;

    case 18:
        return HYGON;

    case 19:
        return RDC;

    case 20:
        return ELBRUS;

    case 21:
        return QEMU;

    case 22:
        return KVM;

    case 23:
        return VMWARE;

    case 24:
        return VIRTUALBOX;

    case 25:
        return XEN;

    case 26:
        return HYPERV;

    case 27:
        return MICROSOFT_XTA;

    case 28:
        return APPLE_ROSETTA_2;

    case 29:
    case 30:
        return PARALLELS;

    case 31:
        return BHYVE;

    case 32:
        return QNX;

    default:
        return UNKNOWN;
    }
}

static void cpu_print_brand_name(enum cpu_brand brand) {
    info("brand: ");
    switch (brand) {
    case AMD:
        info("AMD\n");
        return;

    case INTEL:
        info("Intel\n");
        return;

    case VIA:
        info("VIA\n");
        return;

    case TRANSMETA:
        info("Transmeta\n");
        return;

    case CYRIX:
        info("Cyrix\n");
        return;

    case CENTAUR:
        info("Centaur\n");
        return;

    case NEXGEN:
        info("NexGen\n");
        return;

    case UMC:
        info("UMC\n");
        return;

    case SIS:
        info("Sis\n");
        return;

    case NSC:
        info("National Semiconductor\n");
        return;

    case RISE:
        info("Rise\n");
        return;

    case VORTEX:
        info("Vortex86\n");
        return;

    case AO486:
        info("ao486\n");
        return;

    case ZHAOXIN:
        info("Zhaoxin\n");
        return;

    case HYGON:
        info("HYGON\n");
        return;

    case RDC:
        info("RDC Semiconductor\n");
        return;

    case ELBRUS:
        info("MCST Elbrus\n");
        return;

    case QEMU:
        info("QEMU\n");
        return;

    case KVM:
        info("KVM\n");
        return;

    case VMWARE:
        info("VMware\n");
        return;

    case VIRTUALBOX:
        info("VirtualBox\n");
        return;

    case XEN:
        info("Xen\n");
        return;

    case HYPERV:
        info("Hyper-V\n");
        return;

    case MICROSOFT_XTA:
        info("Microsoft x86-to-ARM\n");
        return;

    case APPLE_ROSETTA_2:
        info("Apple Rosetta 2\n");
        return;

    case PARALLELS:
        info("Parallels\n");
        return;

    case BHYVE:
        info("Bhyve\n");
        return;

    case QNX:
        info("QNX\n");
        return;

    case UNKNOWN:
        info("Unknown CPU\n");
        return;
    }

    __builtin_unreachable();
}

static u32 cpu_get_model(void) {
    u32 eax, unused;
    __cpuid(1, eax, unused, unused, unused);

    // u8 stepping_id = eax & 0xf;
    // u8 processor_type = (eax >> 12) & 0x3;
    // u8 familly_id = (eax >> 8) & 0xf;
    // u8 familly_sum_id = familly_id + ((eax >> 20) & 0xff);
    //
    // u8 model_id = (eax >> 4) & 0xf;
    //
    // if (familly_id == 6 || familly_id == 15) {
    //     model_id += ((eax >> 16) & 0xf << 4);
    // }

    // TODO: Figure out what to do with these values
    // info("Stepping ID: 0x%x\n", stepping_id);
    // info("Proc Type: 0x%x\n", processor_type);
    // info("Familly ID: 0x%x\n", familly_sum_id);
    // info("Model ID: 0x%x\n", model_id);

    return eax;
}

static void cpu_print_cache_tlb_info(void) {
    u32 eax, ebx, ecx, edx;
    __cpuid(2, eax, ebx, ecx, edx);

    if (eax != 1) {
        info("Unsupported cache/tlb query\n");
        return;
    }

    // TODO: Fetch each byte in this table
    // https://en.wikipedia.org/wiki/CPUID#EAX=2:_Cache_and_TLB_Descriptor_information
    // if (!(ebx >> 31)) {
    // }
    //
    // if (!(ecx >> 31)) {
    // }
    //
    // if (!(edx >> 31)) {
    // }
}

static void cpu_print_processor_brand_string(void) {
    u32 regs[12];
    char str[sizeof(regs) + 1];

    __cpuid(0x80000000, regs[0], regs[1], regs[2], regs[3]);

    if (regs[0] < 0x80000004)
        return;

    __cpuid(0x80000002, regs[0], regs[1], regs[2], regs[3]);
    __cpuid(0x80000003, regs[4], regs[5], regs[6], regs[7]);
    __cpuid(0x80000004, regs[8], regs[9], regs[10], regs[11]);

    memcpy(str, regs, sizeof(regs));
    str[sizeof(regs)] = '\0';
    info("model: %s\n", str);
}

bool cpu_check_feature_ecx(enum cpuid_features_ecx feature) {
    u32 unused, ecx;
    __cpuid(1, unused, unused, ecx, unused);
    return ecx & feature;
}

bool cpu_check_feature_edx(enum cpuid_features_edx feature) {
    u32 unused, edx;
    __cpuid(1, unused, unused, unused, edx);
    return edx & feature;
}

void cpu_print_info(void) {
    info("\nCPU INFO:\n");
    cpu_print_brand_name(cpu_get_brand());
    cpu_print_processor_brand_string();
    cpu_get_model();
    cpu_print_cache_tlb_info();
    info("\n");
}
