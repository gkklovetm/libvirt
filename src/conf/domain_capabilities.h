/*
 * domain_capabilities.h: domain capabilities XML processing
 *
 * Copyright (C) 2014 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Author: Michal Privoznik <mprivozn@redhat.com>
 */

#ifndef __DOMAIN_CAPABILITIES_H__
# define __DOMAIN_CAPABILITIES_H__

# include "internal.h"
# include "domain_conf.h"

typedef const char * (*virDomainCapsValToStr)(int value);

typedef struct _virDomainCaps virDomainCaps;
typedef virDomainCaps *virDomainCapsPtr;

typedef struct _virDomainCapsEnum virDomainCapsEnum;
typedef virDomainCapsEnum *virDomainCapsEnumPtr;
struct _virDomainCapsEnum {
    unsigned int values; /* Bitmask of values supported in the corresponding enum */
};

typedef struct _virDomainCapsStringValues virDomainCapsStringValues;
typedef virDomainCapsStringValues *virDomainCapsStringValuesPtr;
struct _virDomainCapsStringValues {
    char **values;  /* raw string values */
    size_t nvalues; /* number of strings */
};

typedef struct _virDomainCapsLoader virDomainCapsLoader;
typedef virDomainCapsLoader *virDomainCapsLoaderPtr;
struct _virDomainCapsLoader {
    bool supported;
    virDomainCapsStringValues values;   /* Info about values for the element */
    virDomainCapsEnum type;     /* Info about virDomainLoader */
    virDomainCapsEnum readonly; /* Info about readonly:virTristateBool */
};

typedef struct _virDomainCapsOS virDomainCapsOS;
typedef virDomainCapsOS *virDomainCapsOSPtr;
struct _virDomainCapsOS {
    bool supported;
    virDomainCapsLoader loader;     /* Info about virDomainLoaderDef */
};

typedef struct _virDomainCapsDeviceDisk virDomainCapsDeviceDisk;
typedef virDomainCapsDeviceDisk *virDomainCapsDeviceDiskPtr;
struct _virDomainCapsDeviceDisk {
    bool supported;
    virDomainCapsEnum diskDevice;   /* Info about virDomainDiskDevice enum values */
    virDomainCapsEnum bus;          /* Info about virDomainDiskBus enum values */
    /* add new fields here */
};

typedef struct _virDomainCapsDeviceGraphics virDomainCapsDeviceGraphics;
typedef virDomainCapsDeviceGraphics *virDomainCapsDeviceGraphicsPtr;
struct _virDomainCapsDeviceGraphics {
    bool supported;
    virDomainCapsEnum type;   /* virDomainGraphicsType */
};

typedef struct _virDomainCapsDeviceVideo virDomainCapsDeviceVideo;
typedef virDomainCapsDeviceVideo *virDomainCapsDeviceVideoPtr;
struct _virDomainCapsDeviceVideo {
    bool supported;
    virDomainCapsEnum modelType;   /* virDomainVideoType */
};

typedef struct _virDomainCapsDeviceHostdev virDomainCapsDeviceHostdev;
typedef virDomainCapsDeviceHostdev *virDomainCapsDeviceHostdevPtr;
struct _virDomainCapsDeviceHostdev {
    bool supported;
    virDomainCapsEnum mode;             /* Info about virDomainHostdevMode */
    virDomainCapsEnum startupPolicy;    /* Info about virDomainStartupPolicy */
    virDomainCapsEnum subsysType;       /* Info about virDomainHostdevSubsysType */
    virDomainCapsEnum capsType;         /* Info about virDomainHostdevCapsType */
    virDomainCapsEnum pciBackend;       /* Info about virDomainHostdevSubsysPCIBackendType */
    /* add new fields here */
};

typedef struct _virDomainCapsFeatureGIC virDomainCapsFeatureGIC;
typedef virDomainCapsFeatureGIC *virDomainCapsFeatureGICPtr;
struct _virDomainCapsFeatureGIC {
    bool supported;
    virDomainCapsEnum version; /* Info about virGICVersion */
};

typedef enum {
    VIR_DOMCAPS_CPU_USABLE_UNKNOWN,
    VIR_DOMCAPS_CPU_USABLE_YES,
    VIR_DOMCAPS_CPU_USABLE_NO,

    VIR_DOMCAPS_CPU_USABLE_LAST
} virDomainCapsCPUUsable;
VIR_ENUM_DECL(virDomainCapsCPUUsable);

typedef struct _virDomainCapsCPUModel virDomainCapsCPUModel;
typedef virDomainCapsCPUModel *virDomainCapsCPUModelPtr;
struct _virDomainCapsCPUModel {
    char *name;
    virDomainCapsCPUUsable usable;
};

typedef struct _virDomainCapsCPUModels virDomainCapsCPUModels;
typedef virDomainCapsCPUModels *virDomainCapsCPUModelsPtr;
struct _virDomainCapsCPUModels {
    virObject parent;

    size_t nmodels_max;
    size_t nmodels;
    virDomainCapsCPUModelPtr models;
};

typedef struct _virDomainCapsCPU virDomainCapsCPU;
typedef virDomainCapsCPU *virDomainCapsCPUPtr;
struct _virDomainCapsCPU {
    bool hostPassthrough;
    virCPUDefPtr hostModel;
    virDomainCapsCPUModelsPtr custom;
};

struct _virDomainCaps {
    virObjectLockable parent;

    char *path;                     /* path to emulator binary */
    virDomainVirtType virttype;     /* virtualization type */
    char *machine;                  /* machine type */
    virArch arch;                   /* domain architecture */

    /* Some machine specific info */
    int maxvcpus;

    virDomainCapsOS os;
    virDomainCapsCPU cpu;
    virDomainCapsDeviceDisk disk;
    virDomainCapsDeviceGraphics graphics;
    virDomainCapsDeviceVideo video;
    virDomainCapsDeviceHostdev hostdev;
    /* add new domain devices here */

    virDomainCapsFeatureGIC gic;
    /* add new domain features here */
};

virDomainCapsPtr virDomainCapsNew(const char *path,
                                  const char *machine,
                                  virArch arch,
                                  virDomainVirtType virttype);

virDomainCapsCPUModelsPtr virDomainCapsCPUModelsNew(size_t nmodels);
virDomainCapsCPUModelsPtr virDomainCapsCPUModelsCopy(virDomainCapsCPUModelsPtr old);
virDomainCapsCPUModelsPtr virDomainCapsCPUModelsFilter(virDomainCapsCPUModelsPtr old,
                                                       const char **models,
                                                       const char **blacklist);
int virDomainCapsCPUModelsAddSteal(virDomainCapsCPUModelsPtr cpuModels,
                                   char **name,
                                   virDomainCapsCPUUsable usable);
int virDomainCapsCPUModelsAdd(virDomainCapsCPUModelsPtr cpuModels,
                              const char *name,
                              ssize_t nameLen,
                              virDomainCapsCPUUsable usable);

# define VIR_DOMAIN_CAPS_ENUM_SET(capsEnum, ...)            \
    do {                                                    \
        unsigned int __values[] = {__VA_ARGS__};            \
        size_t __nvalues = ARRAY_CARDINALITY(__values);     \
        virDomainCapsEnumSet(&(capsEnum), #capsEnum,        \
                             __nvalues, __values);          \
    } while (0)

int virDomainCapsEnumSet(virDomainCapsEnumPtr capsEnum,
                         const char *capsEnumName,
                         size_t nvalues,
                         unsigned int *values);
void virDomainCapsEnumClear(virDomainCapsEnumPtr capsEnum);

char * virDomainCapsFormat(virDomainCapsPtr const caps);
#endif /* __DOMAIN_CAPABILITIES_H__ */
