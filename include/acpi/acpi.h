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

typedef struct AcpiGenericAddress {
	uint8_t space_id;		/* Address space where struct or register exists */
	uint8_t bit_width;		/* Size in bits of given register */
	uint8_t bit_offset;		/* Bit offset within the register */
	uint8_t access_width;	/* Minimum Access size (ACPI 3.0) */
	int64_t address;		/* 64-bit address of struct or register */
} __attribute__((packed)) AcpiGenericAddress_t;
typedef struct AcpiFadt {
    AcpiHeader  header;
    int         firmware_ctrl;
    uint32_t    dsdt_addr;
    char        reserved1;
    char        preferred_pm_Profile;
    int16_t     SCI_INT;
    int         SMI_CMD;
    char        ACPI_ENABLE;
    char        ACPI_DISABLE;
    char        S4BIOS_REQ;
    char        PSTATE_CNT;
    int         PM1a_EVT_BLK;
    int         PM1b_EVT_BLK;
    int         PM1a_CNT_BLK;
    int         PM1b_CNT_BLK;
    int         PM2_CNT_BLK;
    int         PM_TMR_BLK;
    int         GPE0_BLK;
    int         GPE1_BLK;
    char        PM1_EVT_LEN;
    char        PM1_CNT_LEN;
    char        PM2_CNT_LEN;
    char        PM_TMR_LEN;
    char        GPE0_BLK_LEN;
    char        GPE1_BLK_LEN;
    char        GPE1_BASE;
    char        CST_CNT;
    int16_t     P_LVL2_LAT;
    int16_t     P_LVL3_LAT;
    int16_t     FLUSH_SIZE;
    int16_t     FLUSH_STRIDE;
    char        DUTY_OFFSET;
    char        DUTY_WIDTH;
    char        DAY_ALRM;
    char        MON_ALRM;
    char        CENTURY;
    int16_t     IAPC_BOOT_ARCH;
    char        reserved2;
    int         flags;
    AcpiGenericAddress_t        RESET_REG;
    char        RESET_VALUE;
    int16_t     ARM_BOOT_ARCH;
    char        fadt_minor_version;
    int64_t     X_FIRMWARE_CTRL;
    int64_t     X_DSDT;
    AcpiGenericAddress_t        X_PM1a_EVT_BLK;
    AcpiGenericAddress_t        X_PM1b_EVT_BLK;
    AcpiGenericAddress_t        X_PM1a_CNT_BLK;
    AcpiGenericAddress_t        X_PM1b_CNT_BLK;
    AcpiGenericAddress_t        X_PM2_CNT_BLK;
    AcpiGenericAddress_t        X_PM_TMR_BLK;
    AcpiGenericAddress_t        X_GPE0_BLK;
    AcpiGenericAddress_t        X_GPE1_BLK;
    AcpiGenericAddress_t        SLEEP_CONTROL_REG;
    AcpiGenericAddress_t        SLEEP_STATUS_REG;
    int64_t     hypervisor_vendor_identity;
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
typedef struct DSDT {
    AcpiHeader header;
    char* block;
} __attribute__((packed)) DSDT_t;

extern unsigned int g_acpiCpuCount;
extern uint8_t g_acpiCpuIds[MAX_CPU_COUNT];

void AcpiInit();
void ACPIPoweroff();