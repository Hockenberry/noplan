ENTRY(_start)
SECTIONS {
    . = 0x40000000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss COMMON) }
    . = ALIGN(8);
    . += 0x1000;
    _stack_top = .;
}