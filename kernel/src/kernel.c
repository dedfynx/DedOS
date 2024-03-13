#include <limine.h>

#include "Common.h"
//#include "gdt.h"

// Set the base revision to 1, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

static volatile LIMINE_BASE_REVISION(1);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once.

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

// Halt and catch fire function.
static void
hcf(void)
{
    asm("cli");
    for (;;)
    {
        asm("hlt");
    }
}

//test long mode
static inline uint64_t rdmsr(uint32_t msr) {
    uint32_t low, high;
    __asm__ __volatile__("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));
    return ((uint64_t)high << 32) | low;
}

int is_long_mode() {
    uint64_t efer = rdmsr(0xC0000080);
    return (efer & (1ULL << 8)) != 0;
}
// The following will be our kernel's entry point.
// If renaming _start() to something else, make sure to change the
// linker script accordingly.
void _start(void)
{
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false)
    {
        hcf();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    size_t H = framebuffer->height;
    size_t W = framebuffer->width;

    for (size_t i = 0; i < H; i++)
    {
        for (size_t j = 0; j < W; j++)
        {
            volatile uint32_t *fb_ptr = framebuffer->address;
            uint8_t red, green, blue;
            red = (j * 2) ^( i * 2);
            blue = i ^ j;
            green = (j * 4) ^ (i * 4);
            fb_ptr[i * (framebuffer->pitch / 4) + j] = (((red << 16) & 0xFF0000)  | ((green << 8)& 0x00FF00) | (blue& 0x0000FF));
        }
    }

    // We're done, just hang...
    hcf();
}