#include "bmp.h"
#include "parser.h"


void CreatBmp(char output_name[], uint64_t** current_sand_pile, int16_t min_x, int16_t max_x, int16_t min_y, int16_t max_y) {
    FILE *file = fopen(output_name, "wb");
    if (!file) {
        std:: cerr << "cannot open bmp file!";
        return;
    }
    BmpFileHeader bfh;
    BmpInfoHeader bih;

    bih.bmp_height = max_y - min_y + 1;
    bih.bmp_width = (((max_x - min_x + 1) + 7) / 8) * 8;
    bfh.bmp_file_size = sizeof(bih) + sizeof(bfh) + sizeof(color_table) + bih.bmp_height * bih.bmp_width / 2;
    bfh.bmp_file_shift = sizeof(bih) + sizeof(bfh) + sizeof(color_table);
    bih.bmp_header_size = sizeof(bih);
    fwrite(&bfh, sizeof(bfh), 1, file);
    fwrite(&bih, sizeof(bih), 1, file);
    fwrite(color_table, sizeof(color_table), 1, file);


    uint8_t** pixel_data = new uint8_t* [bih.bmp_height];
    for (int i = 0; i < bih.bmp_height; ++i) {
        pixel_data[i] = new uint8_t[bih.bmp_width / 2] {};
    }

    uint8_t first_pixel = 0;
    uint8_t second_pixel = 0;

    for (int i = min_y; i <= max_y; ++i) {
        for (int j = min_x; j <= max_x; j += 2) {
            first_pixel = std::min(static_cast<uint64_t>(4), current_sand_pile[i][j]);
            second_pixel = std::min(static_cast<uint64_t>(4), current_sand_pile[i][j + 1]);
            pixel_data[i - min_y][(j - min_x) / 2] = (first_pixel << 4) | second_pixel;
        }
    }


    for (int i = 0; i < bih.bmp_height; ++i) {
        for (int j = 0; j < (bih.bmp_width / 2); ++j) {
            fwrite(&pixel_data[i][j], 1, 1, file);
        }
    }
    fclose(file);

    for (int i = 0; i < bih.bmp_height; ++i) {
        delete[] pixel_data[i];
    }
    delete[] pixel_data;
}
