#include "gdt.h"
#include "tss.h"

#define GDT_SIZE 10

gdt_descriptor_t gdt[GDT_SIZE];
gdt_pointer_t gdt_pointer;

void setSegment(uint8_t index, uint32_t base, uint32_t limit, uint8_t acces, uint8_t granul)
{
    // set the base
    gdt[index].base_low = (base & 0xFFFFU);
    gdt[index].base_middle = (base >> 16U) & 0xFFU;
    gdt[index].base_high = (base >> 24U) & 0xFFU;
    // Set the limit
    gdt[index].limit_low = (limit & 0xFFFFU);
    gdt[index].granularity = (limit >> 16U) & 0x0FU; // limit high
    // set the granularity
    gdt[index].granularity = granul & 0xF0U;
    gdt[index].access = acces;
}

void initGdt()
{
    for (uint32_t it = 0; it < GDT_SIZE; ++it)
    {
        gdt[it].limit_low = 0;
        gdt[it].base_low = 0;
        gdt[it].base_middle = 0;
        gdt[it].access = 0;
        gdt[it].granularity = 0;
        gdt[it].base_high = 0;
    }

    // NULL
    setSegment(0, 0, 0, 0, 0);
    // KERNEL CODE SEGMENT
    setSegment(
        1,
        0,
        0xFFFFFFFF,
        GDT_PRESENT | GDT_KERNEL | GDT_CODE | GDT_RW,
        GDT_GRANULARITY | GDT_OPERAND_SIZE);

    // KERNEL DATA SEGMENT
    setSegment(
        2,
        0,
        0xFFFFFFFF,
        GDT_PRESENT | GDT_KERNEL | GDT_DATA,
        GDT_GRANULARITY | GDT_OPERAND_SIZE);
    // USER   CODE SEGMENT
    setSegment(
        3,
        0,
        0xFFFFFFFF,
        GDT_PRESENT | GDT_USER | GDT_CODE | GDT_RW,
        GDT_GRANULARITY | GDT_OPERAND_SIZE);
    // USER   DATA SEGMENT
    setSegment(
        4,
        0,
        0xFFFFFFFF,
        GDT_PRESENT | GDT_USER | GDT_DATA,
        GDT_GRANULARITY | GDT_OPERAND_SIZE);
    // TSS
    tss_init(5, 0x10);
}