// ------------------------------------------------------------------------------------------------
// acpi/acpi.h
// ------------------------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

#define MAX_CPU_COUNT 16

extern unsigned int g_acpiCpuCount;
extern uint8_t g_acpiCpuIds[MAX_CPU_COUNT];

void AcpiInit();
unsigned int AcpiRemapIrq(unsigned int irq);
