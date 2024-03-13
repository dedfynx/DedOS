#include "tss.h"

#include "gdt.h"

static tss_entry_t kernel_tss;

void tss_init(uint8_t index, uint32_t ss0)
{
    uint32_t base = (uint32_t)&kernel_tss;
    uint32_t limit = base + sizeof(tss_entry_t);

    setSegment(
        index,
        base,
        limit,
        GDT_PRESENT | GDT_EX | GDT_AC, //0x89
        0x0);

    //memset(&kernel_tss, 0x0, sizeof(tss_entry_t));
    kernel_tss.ss0 = ss0;
    kernel_tss.esp0 = 0x0;
    kernel_tss.cs = 0x0b;
    kernel_tss.ds = 0x13;
    kernel_tss.es = 0x13;
    kernel_tss.fs = 0x13;
    kernel_tss.gs = 0x13;
    kernel_tss.ss = 0x13;
    kernel_tss.iomap = sizeof(tss_entry_t);
}