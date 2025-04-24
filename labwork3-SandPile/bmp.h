#include <cstdint>
#include <iostream>


#pragma pack(push, 1)
struct BmpFileHeader {
    uint16_t bmp_file_type = 0x4D42;
    uint32_t bmp_file_size = 0;
    uint16_t bmp_reserved1 = 0;
    uint16_t bmp_reserved2 = 0;
    uint32_t bmp_file_shift = 0;
};

struct BmpInfoHeader{
    uint32_t bmp_header_size = 0;
    int32_t  bmp_width;
    int32_t  bmp_height;
    uint16_t bmp_planes = 1;
    uint16_t bmp_bit_per_pixel = 4;
    uint32_t bmp_compression = 0;
    uint32_t bmp_size_image = 0;
    int32_t  bmp_x_pixel_per_metr = 0;
    int32_t  bmp_y_pixel_per_metr = 0;
    uint32_t bmp_clr_used = 16;
    uint32_t bmp_important_clr = 0;
};
#pragma pack(pop)

const uint8_t color_table[16][4] = {
    {255, 255, 255, 0}, 
    {0, 230, 0, 0},
    {230, 0, 230, 0}, 
    {0, 240, 255, 0}, 
    {0, 0, 0, 0}, 
};


void CreatBmp(char output_name[256], uint64_t** current_sand_pile, int16_t min_x, int16_t max_x, int16_t min_y, int16_t max_y);



