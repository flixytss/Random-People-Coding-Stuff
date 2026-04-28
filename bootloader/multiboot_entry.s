; defines for our stack, and for a return value from the multiboot
; bootloader
STACK_SIZE equ 16384

; define the "kernel_main" function, which this code is going to call
extern kernel_main

section .bss
    align 4096
stack_bottom:
    resb STACK_SIZE ; 16 KiB
stack_top:

section .text

hang:
.hlt_loop:
  hlt
  jmp .hlt_loop

global _entry

_entry:
    ;call check_for_cpuid
    mov esp, stack_top
    push ebx
	push eax
    call kernel_main

    jmp reboot ; If err (the kernel returns) reboots (maybe go into a bootloop)

global check_for_cpuid
extern error_cpuid
check_for_cpuid:
    ; Prior to using the CPUID instruction, we need to make sure the CPU supports it by
    ; testing the 'ID' bit #21 (0x00200000) in the EFLAGS register. If we can flip the ID
    ; bit form '0' to '1' then the CPUID is available in the CPU. We'll start by copying the
    ; current 32-bit contents of EFLAGS register by pushing the entire EFLAGS register onto the stack.

    ; Set the ID bit in EFLAGS, bits 0-15 are 0x0000, bits 16-31 are 0x0000 0000, bits
    pushfd          ; PUSHFD instruction again saves the current state of the EFLAGS register on the stack
                    ; allowing you to modify the flags and later restore them to original state.
    pop eax         ; This will pop the EFLAGS from the stack and load them into the EAX register.
    mov ebx, eax
    xor eax, 1 << 21; Flips the ID bit (bit 21) from '0' to '1' in 0x00200000
    push eax        ; Push the modified EFLAGS back onto the top of the stack.
    popfd           ; Pop EFLAGS with ID bit #21 flipped back into EFLAGS register.

    ; Check if the ID bit is still set
    pushfd          ; Push the modified EFLAGS back onto the stack
    pop eax         ; Pop the modified EFLAGS from the stack back into the EAX register.
    push ebx
    popfd
    cmp eax, ebx    ; Compare EAX to (1 << 21), if they are equal that means it wasn't flipped
                    ; so we know that the CPU supports CPUID.
            ; Restore the saved original EFLAGS from the stack back into EFLAGS register
    je error_cpuid    ; If the comparison fails then the CPU does not support CPUID and jump to process error
    ret             ; Otherwise we will return to the routine
reboot:
    mov al, 0xfe
    out 0x64, al
