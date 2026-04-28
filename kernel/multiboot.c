#include <multiboot2.h>
#include <stdint.h>
#include <stdlib.h>

const multiboot2_parsed_t parse_multiboot2(unsigned long magic, unsigned long addr) {
    multiboot2_parsed_t parsed;

    struct multiboot_tag *tag;
    unsigned size = *(unsigned *) addr;

    multiboot_memory_map_t* mmap; // for case MULTIBOOT_TAG_TYPE_MMAP
    for (tag = (struct multiboot_tag *) (addr + 8); tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + ((tag->size + 7) & ~7))) {
        unsigned best_distance, distance;
        struct multiboot_color *palette;
	   	switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_CMDLINE:
                parsed.cmdline = (struct multiboot_tag_string*)tag;         break;
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                parsed.bootloader_name = (struct multiboot_tag_string*)tag; break;
            case MULTIBOOT_TAG_TYPE_MODULE:
                parsed.module_mod = (struct multiboot_tag_module *)tag;     break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                parsed.mem_info = (struct multiboot_tag_basic_meminfo*)tag; break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                parsed.boot_dev = (struct multiboot_tag_bootdev*)tag;       break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                parsed.mmap = ((struct multiboot_tag_mmap*)tag);            break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                parsed.framebuffer = ((struct multiboot_tag_framebuffer*)tag); break;
            }
	    }
    tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
    parsed.mbi_size = ((unsigned)tag) - addr;

    return parsed;
}
