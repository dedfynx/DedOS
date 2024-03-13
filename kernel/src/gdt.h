#pragma once
#include <stdint.h>
#include <stddef.h>

enum gdt_bits_t {
    /// @brief `0b10000000U` (Present): Allows an entry to refer to a valid segment. This must be 1 for all valid selectors.
    GDT_PRESENT = 128U,
    /// @brief `0b00000000U` (Descriptor Privilege Level): Sets the 2 privilege bits (ring level) to 0 = highest (kernel).
    GDT_KERNEL = 0U,
    /// @brief `0b01100000U` (Descriptor Privilege Level): Sets the 2 privilege bits (ring level) to 3 = lowest (user applications).
    GDT_USER = 96U,
    /// @brief `0b00010000U` (Descriptor): If clear (0) the descriptor defines a system segment (eg. a Task State Segment). If set (1) it defines a code or data segment.
    GDT_S = 16U,
    /// @brief `0b00001000U` (Executable):  If clear (0) the descriptor defines a data segment. If set (1) it defines a code segment which can be executed from.
    GDT_EX = 8U,
    /// @brief `0b00000100U` (Direction/Conforming)
    GDT_DC = 4U,
    /// @brief `0b00000010U` (Readable/Writable)
    GDT_RW = 2U,
    /// @brief `0b00000001U` (Accessed): Just set to 0. The CPU sets this to 1 when the segment is accessed.
    GDT_AC = 1U,
    /// @brief `0b00001100U` (Executable Code): Identifies an executable code segment.
    GDT_CODE = (GDT_S | GDT_EX),
    /// @brief `0b00001001U` (Writable Data): Identifies a writable data segment.
    GDT_DATA = (GDT_S | GDT_RW),
    /// @brief `0b10000000U` (Granularity): If 0 the limit is in 1 B blocks (byte granularity), if 1 the limit is in 4 KiB blocks (page granularity).
    GDT_GRANULARITY = 128U,
    /// @brief `0b01000000U` (Size): If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit protected mode. You can have both 16 bit and 32 bit selectors at once.
    GDT_OPERAND_SIZE = 64U,
};


typedef struct gdt_descriptor
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct gdt_pointer
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_pointer_t;

void initGdt(void);

void setSegment(uint8_t index, uint32_t base, uint32_t limit, uint8_t acces, uint8_t granul);