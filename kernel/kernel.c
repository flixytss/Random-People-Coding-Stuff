//#include "kernel.h"
#include "acpi/acpi.h"
#include "bootoptions.h"
#include "drivers/pci.h"
#include "drivers/tables/isr/isr.h"
#include "fs/fs.h"
#include "pagefault.h"
#include "terminal/printf.h"
#include <drivers/tables/gdt/gdt.h>
#include <drivers/tables/idt/idt.h>
#include <drivers/tables/idt/idt.h>
#include <drivers/tables/irq/irq.h>
#include <drivers/tables/timer/timer.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>
#include <drivers/drives.h>
#include <layouts/kb_layouts.h>
#include <mem.h>
#include <multiboot2.h>
#include <ports.h>
#include <string.h>
#include <terminal/terminal.h>
#include <commands.h>       // Included by Ember2819: Adds commands
#include <colors.h>         // Added by MorganPG1 to centralise colors into one file
#include <users.h>    // ember2819: user & permission system
#include <stdint.h>
#include <users.h>
#include <cpuid.h>
#include <mem/vmm.h>
#include <mem/pmm.h>

#define DEBUG
#define PMM_SIZE multiboot.mem_info->mem_lower + multiboot.mem_info->mem_upper // 4096 * 40

void process_input(unsigned char *buffer) {
    run_command(buffer, TERM_COLOR);
}

static void kmain();

// Move to it own dedicated file
void error_cpuid() {
    printkf("CPUID is not supported in your system!\nHalting forever\n");
    loop
}
extern void check_for_cpuid();

__attribute__((section(".text")))
void kernel_main(unsigned long magic, unsigned long addr) {
    // zero_bss();
// Multiboot2
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        printkf("Invalid magic number: 0x%x\n", (unsigned)magic);
        loop
    }

    if (addr & 7) {
        printkf("Unaligned mbi: 0x%x\n", addr);
        loop
    }
    check_for_cpuid(); // Checks for cpuid support

    // Parse the address passed from the multiboot2
    const multiboot2_parsed_t multiboot = parse_multiboot2(magic, addr);
    AcpiInit();

// Memory
    pmm_init(_kernel_end, PMM_SIZE); // Just a test
    pmm_map_region(_kernel_end, PMM_SIZE); // Same
    register_isr_handler(page_fault, 14);
    int status = init_virtual_memory_manager();
    kmalloc_init();

    #ifdef DEBUG
        // The prints shortcuts are two function being called, so you need to use keys in a loop
        if (status) { EPRINT("VMM Returned %d, VMM failed (Ram lower than 129mb)\n", status); }
        else { OPRINT("VMM Returned %d, VMM was initialized successfully\n", status); }
        IPRINT("Total RAM %dkb\n", PMM_SIZE);
    #endif

    /* Print shortcuts */
    // EPRINT("ERROR\n");
    // IPRINT("INFO\n");
    // WPRINT("WARNING\n");
    // OPRINT("OK\n");

    // Setup keyboard layouts
    set_layout(LAYOUTS[0]);

    init_gdt();
    init_idt();
    irq_install();
    timer_install();
    keyboard_install();
    timer_phase(50);
    drives_init();
    users_init();
    // enumerate_pci();

    get_kdrive(0);
    struct kdrive_t* drive;
    if ((drive = get_kdrive(1)) == 0x0) {
        WPRINT("No slave drive found. Is there any drive attached as a second drive?\n");
    }
    if (drive != 0x0) {
        if ((fs = fs_drive_open(drive)) == 0) {
            WPRINT("Filesystem mount failed. Is that drive a valid FAT16 image?\n");
        } else {
            OPRINT("Filesystem mounted successfully.\n");
        }
    }

    // Welcome display
    terminal_clear(TERM_COLOR);
    printc("----- GeckoOS v1.2 -----\n", TERM_COLOR);
    printc("Built by random people on the internet.\n", TERM_COLOR);
    printkf("User system initialised. Default accounts: root / guest\n");

    kmain();

    printkf("\nReturned from main function, Rebooting...\n");
    if (drive) free(drive->partitions.partitions);
    reboot();
}

static void kmain()
{
    do_login_prompt();

    // int ebx, unused;
    // __cpuid(0, unused, ebx, unused, unused);
    // printkf("%s\n", &multiboot->boot_loader_name);

    while (1) {
        // Build the prompt: "username> "
        user_t *u = users_current();
        if (u) {
            uint8_t pcolor = (u->ring == RING_ADMIN) ? VGA_COLOR_LIGHT_RED : PROMPT_COLOR;
            printc(u->name, pcolor);
            printc("> ", pcolor);
        } else {
            // Shouldn't reach here, but be safe
            printc("> ", PROMPT_COLOR);
        }

        unsigned char buff[512];
        input(buff, 512, TERM_COLOR);
        if (strcmp(buff, "return") == 0) break;
        process_input(buff);
    }
    return;
}
