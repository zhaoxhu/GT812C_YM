STARTUP(vectors.o)
ENTRY(reset_vector)
INPUT(extras.o)

GROUP(libtarget.a libgcc.a libsupc++.a)
MEMORY
{
    ram0 : ORIGIN = 0x0, LENGTH = 0x10000
    ram1 : ORIGIN = 0x20000, LENGTH = 0x1fe0000
}

SECTIONS
{
    .debug_aranges 0 : { *(.debug_aranges) } .debug_pubnames 0 : { *(.debug_pubnames) } .debug_info 0 : { *(.debug_info) } .debug_abbrev 0 : { *(.debug_abbrev) } .debug_line 0 : { *(.debug_line) } .debug_frame 0 : { *(.debug_frame) } .debug_str 0 : { *(.debug_str) } .debug_loc 0 : { *(.debug_loc) } .debug_macinfo 0 : { *(.debug_macinfo) } .note.arm.ident 0 : { KEEP (*(.note.arm.ident)) }
    .fixed_vectors (0x0 + 0x20) : { . = .; KEEP (*(.fixed_vectors)) } > ram0
    .rom_vectors 0x20000 : { __rom_vectors_vma = ABSOLUTE(.); . = .; KEEP (*(.vectors)) } > ram1 __rom_vectors_lma = LOADADDR(.rom_vectors);
    .rel.text : { *(.rel.text) *(.rel.text.*) *(.rel.gnu.linkonce.t*) } > ram1 .rela.text : { *(.rela.text) *(.rela.text.*) *(.rela.gnu.linkonce.t*) } > ram1 .rel.data : { *(.rel.data) *(.rel.data.*) *(.rel.gnu.linkonce.d*) } > ram1 .rela.data : { *(.rela.data) *(.rela.data.*) *(.rela.gnu.linkonce.d*) } > ram1 .rel.rodata : { *(.rel.rodata) *(.rel.rodata.*) *(.rel.gnu.linkonce.r*) } > ram1 .rela.rodata : { *(.rela.rodata) *(.rela.rodata.*) *(.rela.gnu.linkonce.r*) } > ram1 .rel.got : { *(.rel.got) } > ram1 .rela.got : { *(.rela.got) } > ram1 .rel.ctors : { *(.rel.ctors) } > ram1 .rela.ctors : { *(.rela.ctors) } > ram1 .rel.dtors : { *(.rel.dtors) } > ram1 .rela.dtors : { *(.rela.dtors) } > ram1 .rel.init : { *(.rel.init) } > ram1 .rela.init : { *(.rela.init) } > ram1 .rel.fini : { *(.rel.fini) } > ram1 .rela.fini : { *(.rela.fini) } > ram1 .rel.bss : { *(.rel.bss) } > ram1 .rela.bss : { *(.rela.bss) } > ram1 .rel.plt : { *(.rel.plt) } > ram1 .rela.plt : { *(.rela.plt) } > ram1 .rel.dyn : { *(.rel.dyn) } > ram1
    .text ALIGN (0x4) : { _stext = ABSOLUTE(.); PROVIDE (__stext = ABSOLUTE(.)); *(.text*) *(.gnu.warning) *(.gnu.linkonce.t.*) *(.init) *(.glue_7) *(.glue_7t) } > ram1 _etext = .; PROVIDE (__etext = .);
    .fini ALIGN (0x4) : { . = .; *(.fini) } > ram1
    .rodata ALIGN (0x4) : { . = .; *(.rodata*) *(.gnu.linkonce.r.*) } > ram1
    .rodata1 ALIGN (0x4) : { . = .; *(.rodata1) } > ram1
    .got ALIGN (0x4) : { . = .; *(.got.plt) *(.got) _GOT1_START_ = ABSOLUTE (.); *(.got1) _GOT1_END_ = ABSOLUTE (.); _GOT2_START_ = ABSOLUTE (.); *(.got2) _GOT2_END_ = ABSOLUTE (.); } > ram1
    .fixup ALIGN (0x4) : { . = .; *(.fixup) } > ram1
    .gcc_except_table ALIGN (0x4) : { . = .; *(.gcc_except_table) } > ram1
    .data ALIGN (0x4) : { __ram_data_start = ABSOLUTE (.); *(.data*) *(.data1) *(.gnu.linkonce.d.*) . = ALIGN (4); KEEP(*( SORT (.ecos.table.*))) ; . = ALIGN (4); __CTOR_LIST__ = ABSOLUTE (.); KEEP (*(SORT (.ctors*))) __CTOR_END__ = ABSOLUTE (.); __DTOR_LIST__ = ABSOLUTE (.); KEEP (*(SORT (.dtors*))) __DTOR_END__ = ABSOLUTE (.); *(.dynamic) *(.sdata*) *(.gnu.linkonce.s.*) . = ALIGN (4); *(.2ram.*) } > ram1 __rom_data_start = LOADADDR (.data); __ram_data_end = .; PROVIDE (__ram_data_end = .); _edata = .; PROVIDE (edata = .); PROVIDE (__rom_data_end = LOADADDR (.data) + SIZEOF(.data));
    .bss ALIGN (0x4) : { __bss_start = ABSOLUTE (.); *(.scommon) *(.dynsbss) *(.sbss*) *(.gnu.linkonce.sb.*) *(.dynbss) *(.bss*) *(.gnu.linkonce.b.*) *(COMMON) __bss_end = ABSOLUTE (.); } > ram1
    . = ALIGN(4); _end = .; PROVIDE (end = .);
}
