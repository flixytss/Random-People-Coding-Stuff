// ------------------------------------------------------------------------------------------------
// intr/ioapic.c
// ------------------------------------------------------------------------------------------------

#include "ports.h"
#include "terminal/printf.h"
#include <stdint.h>

// ------------------------------------------------------------------------------------------------
// Globals
uint8_t *g_ioApicAddr;

// ------------------------------------------------------------------------------------------------
// Memory mapped registers for IO APIC register access
#define IOREGSEL                        0x00
#define IOWIN                           0x10

// ------------------------------------------------------------------------------------------------
// IO APIC Registers
#define IOAPICID                        0x00
#define IOAPICVER                       0x01
#define IOAPICARB                       0x02
#define IOREDTBL                        0x10

// ------------------------------------------------------------------------------------------------
static void IoApicOut(uint8_t *base, uint8_t reg, uint32_t val)
{
    outl((uint16_t)(base + IOREGSEL), reg);
    outl((uint16_t)(base + IOWIN), val);
}

// ------------------------------------------------------------------------------------------------
static uint32_t IoApicIn(uint8_t *base, uint8_t reg)
{
    outl((uint16_t)(base + IOREGSEL), reg);
    return inl((uint16_t)(base + IOWIN));
}

// ------------------------------------------------------------------------------------------------
void IoApicSetEntry(uint8_t *base, uint8_t index, uint64_t data)
{
    IoApicOut(base, IOREDTBL + index * 2, (uint32_t)data);
    IoApicOut(base, IOREDTBL + index * 2 + 1, (uint32_t)(data >> 32));
}

// ------------------------------------------------------------------------------------------------
void IoApicInit()
{
    // Get number of entries supported by the IO APIC
    uint32_t x = IoApicIn(g_ioApicAddr, IOAPICVER);
    unsigned int count = ((x >> 16) & 0xff) + 1;    // maximum redirection entry

    printkf("I/O APIC pins = %d\n", count);

    // Disable all entries
    for (unsigned int i = 0; i < count; ++i)
    {
        IoApicSetEntry(g_ioApicAddr, i, 1 << 16);
    }
}
