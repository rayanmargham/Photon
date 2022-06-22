#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../serial/serial.h"
#include "../../limine.h"
#include "../utils.h"
#include "fonts/linux_font_8x16.h"
#include "../memory/heap.h"
#include "../memory/pmm.h"
#include "graphics.h"
extern struct limine_framebuffer_request framebuffer_request;
struct GBuffer Backbuffer;


void graphics_init(int screen)
{
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[screen];
    Backbuffer.width = framebuffer->width;
    Backbuffer.height = framebuffer->height;
    Backbuffer.pitch = framebuffer->pitch;
    Backbuffer.buffer = malloc(Backbuffer.height * Backbuffer.pitch);
    flush_backbuffer();
    swap_buffers();
}

void flush_backbuffer()
{
    memset(Backbuffer.buffer, 0, framebuffer_request.response->framebuffers[0]->width * Backbuffer.pitch);
}
void swap_buffers()
{
    memcpy32((uint32_t*)framebuffer_request.response->framebuffers[0]->address, Backbuffer.buffer, Backbuffer.height * Backbuffer.pitch);
}

void blit(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint32_t *src_buffer, uint64_t src_width, uint32_t *dst_buffer, uint64_t dst_width)
{
    for (uint64_t i = 0; i < height; i++)
    {
        for (uint64_t j = 0; j < width; j++)
        {
            dst_buffer[(y + i) * (dst_width) + (x + j)] = src_buffer[i * (src_width) + j];
        }
    }
}
void draw_char(uint64_t x, uint64_t y, char c, uint64_t color, uint32_t* buffer, uint64_t width)
{
    unsigned char* font = (unsigned char*)fontdata_8x16.data;
    for (uint64_t i = 0; i < 16; i++)
    {
        for (uint64_t j = 0; j < 8; j++)
        {
            if ((i + y) >= width || (j + x) >= width)
            {
                continue;;
            }
            if ((font[(c * 16) + i] & (0x80 >> j)) != 0)
            {
               buffer[(i + y) * width + (j + x)] = color;
            }
        }
    }
}


void put_string(uint64_t x, uint64_t y, char* str, uint64_t color, uint32_t* buffer, uint64_t width)
{
    int len;
    len = strlen(str);
    for (uint64_t i = 0; i < len; i++)
    {
        
        if (str[i] == '\n')
        {
            continue;   
        }
        draw_char(x + (i * 8), y, str[i], color, buffer, width);
    }
}
