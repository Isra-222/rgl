ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0x400000;

    .text : { 
        *(.text*) 
    } :text

    .rodata : { 
        *(.rodata*) 
    } :text

    .init_array : {
        __init_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP(*(.init_array .ctors))
        __init_array_end = .;
    } :text

    .fini_array : {
        __fini_array_start = .;
        KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP(*(.fini_array .dtors))
        __fini_array_end = .;
    } :text

    .data : {
        *(.data)
        *(.data.*)
    } :data

    .tdata : {
        __tdata_start = .;
        *(.tdata)
        *(.tdata.*)
        __tdata_end = .;
    } :data

    .tbss : {
        *(.tbss)
        *(.tbss.*)
        __tbss_end = .;
    } :data

    .bss : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    } :data
}