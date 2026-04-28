#include "pagefault.h"
#include "terminal/printf.h"
#include <drivers/tables/isr/isr.h>
#include <terminal/terminal.h>

const char *excname[] = {
  "div err",
  "debug",
  "nmi",
  "breakdown", // i.e. breakpoint
  "overflow",
  "bound range err",
  "invalid opcode",
  "invalid device",
  "double fault",
  "coproc seg overrun",
  "invalid tss",
  "segment not presnt",
  "stack segment",
  "general protection fault",
  "page fault",
  "res",
  "x87 float exc",
  "alignment check",
  "machine check",
  "simd float exc",
  "virtualization exc",
  "control protection exc",
};
isr_t isr_handlers[32];

void isr_handler(registers_t regs)
{
    isr_t e = isr_handlers[regs.int_no];
    if (!e) {
        printkf("interrupt at eip=%p, exc %d (%s)\n", regs.eip, regs.int_no, regs.int_no < sizeof(excname)/sizeof(excname[0]) ? excname[regs.int_no] : "out of bounds");
        printkf("no exception handler! halting now...\n");
        asm volatile("cli");
        while (1) asm volatile("hlt");
    }
    e(&regs);
}
void register_isr_handler(isr_t r, uint8_t no) {
  isr_handlers[no] = r;
}
