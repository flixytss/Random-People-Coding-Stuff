ENTRY (_entry)

SECTIONS
{
    g_localApicAddr = 0x00006000;
    g_activeCpuCount = 0x00006008;

    . = 1M;

	_kernel_start = .;
    .multiboot.data : {
        *(.multiboot.data)
    }

	.text :
	{
		*(.text)
		*(.rodata)
		. = ALIGN(4096);
	}
	.data :
	{
		*(.data)
		__data_end__ = .;
		. = ALIGN(4096);
	}
	.bss :
	{
	    __bss_start__ = .;
		*(COMMON)
		*(.bss)
		__bss_end__ = .;
		. = ALIGN(4096);
	}

	_kernel_end = .;
}
