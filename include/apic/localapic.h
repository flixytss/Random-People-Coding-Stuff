// ------------------------------------------------------------------------------------------------
// intr/local_apic.h
// ------------------------------------------------------------------------------------------------

#pragma once

#include <stdint.h>

extern uint8_t *g_localApicAddr;

void LocalApicInit();

unsigned int LocalApicGetId();
void LocalApicSendInit(unsigned int apic_id);
void LocalApicSendStartup(unsigned int apic_id, unsigned int vector);
