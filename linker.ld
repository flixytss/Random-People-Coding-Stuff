ENTRY (_entry)

SECTIONS
{
        . = 1M;
	/* The kernel will live at 3GB + 1MB in the virtual address space, */
	/* which will be mapped to 1MB in the physical address space. */
	/* Note that we page-align the sections. */

	_kernel_start = .;
    .multiboot.data : {
        *(.multiboot.data)
    }
	/* Add a symbol that indicates the start address of the kernel. */
	.text :
	{
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}
	.data :
	{
		*(.data)
		. = ALIGN(4096);
	}
	.bss :
	{
	    __bss_start__ = .;
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
		__bss_end__ = .;
		. = ALIGN(4096);
	}
	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;
}
