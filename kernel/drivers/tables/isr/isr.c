#include "pagefault.h"
#include "terminal/printf.h"
#include <drivers/tables/isr/isr.h>
#include <terminal/terminal.h>

void isr_handler(registers_t regs)
{
    // Idk why does the cpu makes an interruption instead of calling the 14th irq handler that is just for that
    if (regs.int_no == 0xe) {
        // printkf("HERE\n");
        page_fault(&regs);
    }

    print("recieved interrupt: ");
    print_hex(regs.int_no);
    print("\n");
}
