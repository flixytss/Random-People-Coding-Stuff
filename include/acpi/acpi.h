// acpi/acpi.h

#pragma once

#include <stdint.h>

#define MAX_CPU_COUNT 16
#define MAX_IOAPICS   32
#define MAX_INTOVERR  32

typedef struct AcpiHeader
{
    uint32_t signature;
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    uint8_t oem[6];
    uint8_t oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} __attribute__((packed)) AcpiHeader;

typedef struct AcpiFadt
{
    AcpiHeader header;
    uint32_t firmwareControl;
    uint32_t dsdt;
    uint8_t reserved;
    uint8_t preferredPMProfile;
    uint16_t sciInterrupt;
    uint32_t smiCommandPort;
    uint8_t acpiEnable;
    uint8_t acpiDisable;
} __attribute__((packed)) AcpiFadt;

typedef struct AcpiMadt
{
    AcpiHeader header;
    uint32_t localApicAddr;
    uint32_t flags;
} __attribute__((packed)) AcpiMadt;

typedef struct ApicHeader
{
    uint8_t type;
    uint8_t length;
} __attribute__((packed)) ApicHeader;

#define APIC_TYPE_LOCAL_APIC            0
#define APIC_TYPE_IO_APIC               1
#define APIC_TYPE_INTERRUPT_OVERRIDE    2

typedef struct ApicLocalApic
{
    ApicHeader header;
    uint8_t acpiProcessorId;
    uint8_t apicId;
    uint32_t flags;
} __attribute__((packed)) ApicLocalApic;
typedef struct ApicIoApic
{
    ApicHeader header;
    uint8_t ioApicId;
    uint8_t reserved;
    uint32_t ioApicAddress;
    uint32_t globalSystemInterruptBase;
} __attribute__((packed)) ApicIoApic;
typedef struct ApicInterruptOverride
{
    ApicHeader header;
    uint8_t bus;
    uint8_t source;
    uint32_t interrupt;
    uint16_t flags;
} __attribute__((packed)) ApicInterruptOverride;

extern unsigned int g_acpiCpuCount;
extern uint8_t g_acpiCpuIds[MAX_CPU_COUNT];

#define ACPIPROC_NON_DEFINED    0xff

typedef struct ACPIProcessor {
    uint8_t         processorId;
    uint8_t         apicId;
    uint32_t        flags;
} ACPIProcessor_t;
typedef struct IOApic {
    uint8_t         ioApicId;
    uint32_t        ioApicAddress;
    uint32_t        globalSystemInterruptBase;
} IOApic_t;
typedef struct INTOverride {
    uint8_t         bus;
    uint8_t         source;
    uint32_t        interrupt;
    uint16_t        flags;
} INTOverride_t;

typedef struct ACPIInfo {
    uint32_t        localApicAddr;
    ACPIProcessor_t processors[MAX_CPU_COUNT];
    IOApic_t        ioapics[MAX_IOAPICS];
    INTOverride_t   intoverrides[MAX_INTOVERR];

    AcpiFadt*       fadt;
    AcpiMadt*       madt;
    AcpiHeader*     acpiHeader;
    uint32_t        rsdp;
} ACPIInfo_t;

ACPIInfo_t AcpiInit();
uint32_t AcpiRemapIrq(unsigned int irq);
