// acpi/acpi.c

#include "drivers/tables/timer/timer.h"
#include "drivers/vga.h"
#include "ports.h"
#include <acpi/acpi.h>
#include <ioapic/ioapic.h>
#include <apic/localapic.h>
#include <string.h>
#include <terminal/printf.h>
#include <stdint.h>
#include <terminal/terminal.h>

unsigned int g_acpiCpuCount = 0;
uint8_t g_acpiCpuIds[MAX_CPU_COUNT];
static AcpiMadt *s_madt;
static AcpiFadt* g_facp;

uint32_t SLP_TYPa, SLP_TYPb;

static void AcpiParseFacp(AcpiFadt *facp)
{
    g_facp = facp;
    if (facp->SMI_CMD)
    {
        IPRINT("Enabling ACPI at 0x%x (Which value's %x) with 0x%x\n", facp->SMI_CMD, inb(facp->SMI_CMD), facp->ACPI_ENABLE);
        outb(facp->SMI_CMD, facp->ACPI_ENABLE);
        sleep(3);
        while ((inw(facp->PM1a_CNT_BLK) & 1) == 0); // Waits for the bit to change
        IPRINT("ACPI control value: %x\n", inb(facp->PM1a_CNT_BLK));
    }
}

static void AcpiParseApic(AcpiMadt *madt)
{
    s_madt = madt;

    IPRINT("Local APIC Address = 0x%08x\n", madt->localApicAddr);
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
                ++g_acpiCpuCount;
            }
        }
        else if (type == APIC_TYPE_IO_APIC)
        {
            ApicIoApic *s = (ApicIoApic *)p;

            IPRINT("Found I/O APIC: %d 0x%08x %d\n", s->ioApicId, s->ioApicAddress, s->globalSystemInterruptBase);
            g_ioApicAddr = (uint8_t *)(uintptr_t)s->ioApicAddress;
        }
        else if (type == APIC_TYPE_INTERRUPT_OVERRIDE)
        {
            ApicInterruptOverride *s = (ApicInterruptOverride *)p;

            IPRINT("Found Interrupt Override: %d %d %d 0x%04x\n", s->bus, s->source, s->interrupt, s->flags);
        }
        else
        {
            WPRINT("Unknown APIC structure %d\n", type);
        }

        p += length;
    }
}

static void AcpiParseDT(AcpiHeader *header)
{
    if (header->signature == 0x50434146)
    {
        AcpiParseFacp((AcpiFadt *)header);
    }
    else if (header->signature == 0x43495041)
    {
        AcpiParseApic((AcpiMadt *)header);
    }
}

static void AcpiParseRsdt(AcpiHeader *rsdt)
{
    uint32_t *p = (uint32_t *)(rsdt + 1);
    uint32_t *end = (uint32_t *)((uint8_t*)(rsdt) + rsdt->length);

    while (p < end)
    {
        uint32_t address = *p++;
        AcpiParseDT((AcpiHeader *)(uintptr_t)address);
    }
}

static void AcpiParseXsdt(AcpiHeader *xsdt)
{
    uint64_t *p = (uint64_t *)(xsdt + 1);
    uint64_t *end = (uint64_t *)((uint8_t*)xsdt + xsdt->length);

    while (p < end)
    {
        uint64_t address = *p++;
        AcpiParseDT((AcpiHeader *)(uintptr_t)address);
    }
}

static bool AcpiParseRsdp(uint8_t *p)
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
        AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
        // loop
    }
    else if (revision == 2)
    {
        IPRINT("ACPI ver: 2\n");

        uint32_t rsdtAddr = *(uint32_t *)(p + 16);
        uint64_t xsdtAddr = *(uint64_t *)(p + 24);

        if (xsdtAddr)
        {
            AcpiParseXsdt((AcpiHeader *)(uintptr_t)xsdtAddr);
        }
        else
        {
            AcpiParseRsdt((AcpiHeader *)(uintptr_t)rsdtAddr);
        }
    }
    else
    {
        EPRINT("Unsupported ACPI version %d\n", revision);
    }

    return true;
}

void AcpiInit()
{
    // TODO - Search Extended BIOS Area

    // Search main BIOS area below 1MB
    uint8_t *p = (uint8_t *)0x000e0000;
    uint8_t *end = (uint8_t *)0x000fffff;

    // Search in the BIOS extended memory
    int ebda = *((short *) 0x40E);
	ebda = ebda * 0x10 &0x000FFFFF;

	for (unsigned int * addr = (unsigned int *) ebda; (int) addr<ebda+1024; addr+= 0x10/sizeof(addr)) {
	    uint64_t signature = *(uint64_t*)addr;
	    if (signature == 0x2052545020445352) // 'RSD PTR '
        {
            IPRINT("ACPI is in BIOS extended memory\n");
            if (AcpiParseRsdp(p))
            {
                goto l;
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
            if (AcpiParseRsdp(p))
            {
                goto l;                
            }
        }

        p += 16;
    }
    l:
    printkf("RESET_REG: 0x%x\nSLEEP_REG: 0x%x", g_facp->RESET_REG.address, g_facp->SLEEP_CONTROL_REG.address);
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
void ACPIPoweroff() {
    if (g_facp->ACPI_ENABLE == 0) return;
    outw((unsigned int) g_facp->PM1a_CNT_BLK, g_facp->SLEEP_CONTROL_REG.address | g_facp->ACPI_ENABLE );
	if ( g_facp->PM1b_CNT_BLK != 0 )
		outw((unsigned int) g_facp->PM1b_CNT_BLK, g_facp->SLEEP_STATUS_REG.address | g_facp->ACPI_ENABLE );
}