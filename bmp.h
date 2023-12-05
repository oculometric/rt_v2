#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

struct bmp_header_info
{
    const uint32_t header_size_bytes = 40;
    int32_t bitmap_width_pixels;
    int32_t bitmap_height_pixels;
    const uint16_t colour_planes = 1;
    uint16_t bits_per_pixel;
    uint32_t compression_method = 0;
    uint32_t image_size = 0;
    int32_t pixels_per_meter_horizontal = 2835;
    int32_t pixels_per_meter_vertical = 2835;
    uint32_t colours_in_palette = 0;
    uint32_t important_colours = 0;
};

static void output_uint32(uint32_t t, ofstream & stream)
{
    char t_0 = t & 0xFF;
    char t_1 = (t >> 8) & 0xFF;
    char t_2 = (t >> 16) & 0xFF;
    char t_3 = (t >> 24) & 0xFF;
    stream << t_0 << t_1 << t_2 << t_3;
}

static void output_uint16(uint16_t t, ofstream & stream)
{
    char t_0 = t & 0xFF;
    char t_1 = (t >> 8) & 0xFF;
    stream << t_0 << t_1;
}

static bool write_bmp(const uint8_t * buffer, uint32_t width, uint32_t height, uint8_t bytes_per_pixel, const char * file_path)
{
    if (width == 0 || height == 0) return false;
    if (buffer == NULL) return false;
    if (bytes_per_pixel != 1 && bytes_per_pixel != 3) return false;

    ofstream file;
    file.open(file_path);
    if (!file.is_open()) return false;

    uint32_t row_size_bytes = ceil((float)(bytes_per_pixel * width) / 4.0f) * 4;
    uint32_t pixel_array_size = row_size_bytes * height;

    // beginning of the bitmap file header
    // 'BM' magic number
    file << (char)0x42 << (char)0x4D;

    // file size in bytes: pixel buffer, headers, index table
    uint32_t file_size = pixel_array_size + 54;
    output_uint32(file_size, file);

    // reserved
    char zero = 0;
    file << zero << zero;

    // reserved
    file << zero << zero;

    // offset of the pixel array
    uint32_t pixel_array_offset = 54;
    output_uint32(pixel_array_offset, file);

    // beginning of the DIB bitmap information header (BITMAPINFOHEADER)
    bmp_header_info header;
    header.bitmap_width_pixels = width;
    header.bitmap_height_pixels = -height;
    header.bits_per_pixel = 24;

    // size of header in bytes
    output_uint32(header.header_size_bytes, file);
    
    // bitmap width in pixels
    output_uint32(header.bitmap_width_pixels, file);

    // bitmap height in pixels
    output_uint32(header.bitmap_height_pixels, file);

    // bitmap color planes
    output_uint16(header.colour_planes, file);
    
    // bits per pixel
    output_uint16(header.bits_per_pixel, file);

    // compression method used
    output_uint32(header.compression_method, file);

    // image size
    output_uint32(header.image_size, file);

    // horizontal resolution
    output_uint32(header.pixels_per_meter_horizontal, file);

    // vertical resolution
    output_uint32(header.pixels_per_meter_vertical, file);

    // colours in palette
    output_uint32(header.colours_in_palette, file);

    // important colours used
    output_uint32(header.important_colours, file);

    // beginning of the actual pixel data
    uint8_t padding = (4 - ((width*bytes_per_pixel) % 4)) % 4;
    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            if (bytes_per_pixel == 1)
            {
                file << (char)(buffer[(y * width) + x]);
                file << (char)(buffer[(y * width) + x]);
                file << (char)(buffer[(y * width) + x]);
            } else if (bytes_per_pixel == 3)
            {
                file << (char)(buffer[(((y * width) + x) * 3) + 2]);
                file << (char)(buffer[(((y * width) + x) * 3) + 1]);
                file << (char)(buffer[(((y * width) + x) * 3) + 0]);
            }
        }
        for (uint8_t z = 0; z < padding; z++) file << (char)0x0;
    }

    file.close();
    return true;
}

#endif