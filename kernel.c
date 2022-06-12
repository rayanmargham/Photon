// PHOTON INTERNEL "Wave" Kernel
// Code is governed by the GPL-2.0 license.
#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "src/utils.h"
#include "src/GDT/gdt.h"
#include "src/serial/serial.h"
#include "src/Interrupts/interrupts.h"
#include "src/memory/pmm.h"
#include "src/memory/vmm.h"
#include "src/Graphics/graphics.h"
// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent.
// extern void fill_screen(uint64_t color);
// extern void graphics_init();
extern struct limine_kernel_file_request kernel_file_request;
extern struct limine_terminal_request terminal_request;
static void done(void) {
    for (;;) {
        __asm__("hlt");
    }
};
static uint64_t last_frame_color = 0x000000;
extern void PrepareACPI();
// The following will be our kernel's entry point.
void _start(void) {
    init_serial() != 0 ? done() : serial_print("Hello World!\n");
    
    // We should now be able to call the Limine terminal to print out
    // a simple "Hello World" to screen.
    uint64_t gdt = init_gdt();
    serial_print("GDT loaded\n");
    init_idt();
    serial_print("IDT loaded\n");
    
    pmm_init();
    serial_print("PMM loaded\n");
    // free memory in kilobytes
    //kpanic("don don", sizeof("don don"));
    //graphics_init();
    //fill_screen(0x018281);
    // read serial;
    vmm_init();
    //graphics_init();
    
    uint64_t a = available_memory();
    uint64_t b = total_memory();
    serial_print("Available Memory: ");
    
    serial_print(to_hstring(a));
    serial_print(" bytes\n");
    serial_print("Used Memory: ");
    serial_print(to_hstring(b - a));
    serial_print(" bytes\n");
    serial_print("Total Memory: ");
    serial_print(to_hstring(b));
    serial_print(" bytes\n");
    serial_print("Kernel ARGS: ");
    serial_print(kernel_file_request.response->kernel_file->cmdline);
    serial_print("\n");
    terminal_request.response->write(terminal_request.response->terminals[0], "no limine?\n", 12);
    //put_string(0, 0, "Good Morning sir!", 0xFFFFFF);
    
    //PrepareACPI();
    done();
};