// acpi/acpi.c

#include "drivers/tables/timer/timer.h"
#include "ports.h"
#include <acpi/acpi.h>
#include <ioapic/ioapic.h>
#include <apic/localapic.h>
#include <string.h>
#include <terminal/printf.h>
#include <stdint.h>

unsigned int g_acpiCpuCount = 0;
uint8_t g_acpiCpuIds[MAX_CPU_COUNT];
static AcpiMadt *s_madt;
AcpiFadt g_facp;

static void AcpiParseFacp(AcpiFadt *facp, ACPIInfo_t* log)
{
    g_facp = *facp;
    log->fadt = facp;
    if (facp->smiCommandPort)
    {
        IPRINT("Enabling ACPI at 0x%x (Which value's %x) with 0x%x\n", facp->smiCommandPort, inb(facp->smiCommandPort), facp->acpiEnable);
        outb(facp->smiCommandPort, facp->acpiEnable);
        while ((inb(facp->smiCommandPort) & 1) == 0); // Waits for the bit to change
        IPRINT("ACPI new value: %x\n", inb(facp->smiCommandPort));
    }
    else
    {
        WPRINT("ACPI already enabled\n");
    }
}

static void AcpiParseApic(AcpiMadt *madt, ACPIInfo_t* log)
{
    s_madt = madt;

    IPRINT("Local APIC Address = 0x%08x\n", madt->localApicAddr);
    log->madt = madt;
    log->localApicAddr = madt->localApicAddr;
    g_localApicAddr = (uint8_t *)(uintptr_t)madt->localApicAddr;

    uint8_t *p = (uint8_t *)(madt + 1);
    uint8_t *end = (uint8_t *)madt + madt->header.length;

    uint8_t apics = 0;
    uint8_t overr = 0;
    while (p < end)
    {
        ApicHeader *header = (ApicHeader *)p;
        uint8_t type = header->type;
        uint8_t length = header->length;

        if (type == APIC_TYPE_LOCAL_APIC)
        {
            ApicLocalApic *s = (ApicLocalApic *)p;

            IPRINT("Found CPU: %d %d %x\n", s->acpiProcessorId, s->apicId, s->flags);
            if (g_acpiCpuCount < MAX_CPU_COUNT)
            {
                g_acpiCpuIds[g_acpiCpuCount] = s->apicId;
                log->processors[g_acpiCpuCount] = (ACPIProcessor_t){s->acpiProcessorId, s->apicId, s->flags};
                ++g_acpiCpuCount;
            }
        }
        else if (type == APIC_TYPE_IO_APIC)
        {
            ApicIoApic *s = (ApicIoApic *)p;

            IPRINT("Found I/O APIC: %d 0x%08x %d\n", s->ioApicId, s->ioApicAddress, s->globalSystemInterruptBase);
            log->ioapics[apics++] = (IOApic_t){s->ioApicId, s->ioApicAddress, s->globalSystemInterruptBase};
            g_ioApicAddr = (uint8_t *)(uintptr_t)s->ioApicAddress;
        }
        else if (type == APIC_TYPE_INTERRUPT_OVERRIDE)
        {
            ApicInterruptOverride *s = (ApicInterruptOverride *)p;

            IPRINT("Found Interrupt Override: %d %d %d 0x%04x\n", s->bus, s->source, s->interrupt, s->flags);
            log->intoverrides[overr++] = (INTOverride_t){s->bus, s->source, s->interrupt, s->flags};
        }
        else
        {
            WPRINT("Unknown APIC structure %d\n", type);
        }

        p += length;
    }
}

static void AcpiParseDT(AcpiHeader *header, ACPIInfo_t* log)
{
    uint32_t signature = header->signature;

    char sigStr[5];
    memcpy(sigStr, &signature, 4);
    sigStr[4] = 0;
    // loop

    if (signature == 0x50434146)
    {
        AcpiParseFacp((AcpiFadt *)header, log);
    }
    else if (signature == 0x43495041)
    {
        AcpiParseApic((AcpiMadt *)header, log);
    }
}

static void AcpiParseRsdt(AcpiHeader *rsdt, ACPIInfo_t* log)
{
    uint32_t *p = (uint32_t *)(rsdt + 1);
    uint32_t *end = (uint32_t *)((uint8_t*)(rsdt) + rsdt->length);

    while (p < end)
    {
        uint32_t address = *p++;
        AcpiParseDT((AcpiHeader *)(uintptr_t)address, log);
    }
}

static void AcpiParseXsdt(AcpiHeader *xsdt, ACPIInfo_t* log)
{
    uint64_t *p = (uint64_t *)(xsdt + 1);
    uint64_t *end = (uint64_t *)((uint8_t*)xsdt + xsdt->length);

    while (p < end)
    {
        uint64_t address = *p++;
        AcpiParseDT((AcpiHeader *)(uintptr_t)address, log);
    }
}

static bool AcpiParseRsdp(uint8_t *p, ACPIInfo_t* log)
{
    // Parse Root System Description Pointer
    IPRINT("RSDP found at 0x%x\n", p);

    // Verify checksum
    uint8_t sum = 0;
    for (unsigned int i = 0; i < 20; ++i)
    {
        sum += p[i];
    }

    if (sum)
    {
        EPRINT("Checksum failed\n");
        return false;
    }
    // Print OEM
    char oem[7];
    memcpy(oem, p + 9, 6);
    oem[6] = '\0';
    IPRINT("OEM: %s\n", oem);

    // Check version
    uint8_t revision = p[15];
    if (revision == 0)
    {
        IPRINT("ACPI ver: 1\n");

        uint32_t rsdtAddr = *(uint32_t *)(p + 16);
        // loop
        AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr, log);
        // loop
    }
    else if (revision == 2)
    {
        IPRINT("ACPI ver: 2\n");

        uint32_t rsdtAddr = *(uint32_t *)(p + 16);
        uint64_t xsdtAddr = *(uint64_t *)(p + 24);

        if (xsdtAddr)
        {
            AcpiParseXsdt((AcpiHeader *)(uintptr_t)xsdtAddr, log);
        }
        else
        {
            AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr, log);
        }
    }
    else
    {
        EPRINT("Unsupported ACPI version %d\n", revision);
    }

    return true;
}

ACPIInfo_t AcpiInit()
{
    // TODO - Search Extended BIOS Area

    // Search main BIOS area below 1MB
    uint8_t *p = (uint8_t *)0x000e0000;
    uint8_t *end = (uint8_t *)0x000fffff;

    // Search in the BIOS extended memory
    int ebda = *((short *) 0x40E);
	ebda = ebda * 0x10 &0x000FFFFF;

	ACPIInfo_t info;

	for (unsigned int * addr = (unsigned int *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr)) {
	    uint64_t signature = *(uint64_t*)addr;
	    if (signature == 0x2052545020445352) // 'RSD PTR '
        {
            IPRINT("ACPI is in BIOS extended memory\n");
            if (AcpiParseRsdp(p, &info))
            {
                break;
            }
        }
	}

    while (p < end)
    {
        uint64_t signature = *(uint64_t*)p;
        // printkf("0x%x\n", p);

        if (signature == 0x2052545020445352) // 'RSD PTR '
        {
            IPRINT("ACPI is in BIOS memory\n");
            if (AcpiParseRsdp(p, &info))
            {
                break;
            }
        }

        p += 16;
    }
    return info;
}

uint32_t AcpiRemapIrq(unsigned int irq)
{
    AcpiMadt *madt = s_madt;

    uint8_t *p = (uint8_t *)(madt + 1);
    uint8_t *end = (uint8_t *)madt + madt->header.length;

    while (p < end)
    {
        ApicHeader *header = (ApicHeader *)p;
        uint8_t type = header->type;
        uint8_t length = header->length;

        if (type == APIC_TYPE_INTERRUPT_OVERRIDE)
        {
            ApicInterruptOverride *s = (ApicInterruptOverride *)p;

            if (s->source == irq)
            {
                return s->interrupt;
            }
        }

        p += length;
    }

    return irq;
}
