#include "sandpile.h"
#include "bmp.h"


InitialPile ReadingBmp(char input_name[]) {
    InitialPile result;
    std::ifstream input_file(input_name, std::ios::binary);
    if (!input_file) {
        std:: cerr << "cannot open input bmp file";
        return result;
    }
    BmpFileHeader bfh;
    BmpInfoHeader bih;

    input_file.read(reinterpret_cast<char*>(&bfh), sizeof(bfh));
    input_file.read(reinterpret_cast<char*>(&bih), sizeof(bih));
    input_file.seekg(bfh.bmp_file_shift, std::ios::beg);

    uint64_t bytes_in_line = bih.bmp_width * 3;
    uint32_t padding = 4 - bytes_in_line % 4;
    bytes_in_line += padding;
    
    uint64_t** initial_sand_pile = new uint64_t*[bih.bmp_height];
    for (int i = 0; i < (bih.bmp_height); ++i) {
        initial_sand_pile[i] = new uint64_t[bih.bmp_width];
    }
    for (int i = 0; i < bih.bmp_height; ++i) {
        for (int j = 0; j < bih.bmp_width; ++j) {
            initial_sand_pile[i][j] = 0;
        }
    }
    uint8_t blue_number = 0;    
    uint8_t green_number = 0; 
    uint8_t red_number = 0;


    for(int i = 0; i < bih.bmp_height; ++i) {
        uint8_t* line = new uint8_t[bytes_in_line]; 
        input_file.read(reinterpret_cast<char*>(line), bytes_in_line);
        for(int j = 0; j < bih.bmp_width; ++j) {
            if (j * 3 + 2 < bytes_in_line) {
                blue_number = line[j * 3];    
                green_number = line[j * 3 + 1]; 
                red_number = line[j * 3 + 2];
                initial_sand_pile[i][j] = blue_number + green_number + red_number;
            }
        }
    }

    input_file.close();
    result.sand_pile = initial_sand_pile;
    result.lines = bih.bmp_height;
    result.columns = bih.bmp_width;
    return result; 
}

InitialPile ReadingTsv(char input_name[]) {
    InitialPile result;
    std::ifstream input_file(input_name);
    if (!input_file) {
        std:: cerr << "cannot open input tsv file";
        return result;
    }
    int16_t x = 0;
    int16_t min_x = std::numeric_limits<int16_t>::max();
    int16_t max_x = std::numeric_limits<int16_t>::min();
    int16_t y = 0;
    int16_t min_y = std::numeric_limits<int16_t>::max();
    int16_t max_y = std::numeric_limits<int16_t>::min();
    uint64_t sand_number;
    uint32_t last_tab = 0;
    while (input_file >> x >> y >> sand_number) {
        max_x = std::max(x, max_x);
        min_x = std::min(x, min_x);
        max_y = std::max(y, max_y);
        min_y = std::min(y, min_y);
    }
    input_file.clear();
    input_file.seekg(0);

    uint64_t** initial_sand_pile = new uint64_t*[max_y - min_y + 1];
    for (int i = 0; i < (max_y - min_y + 1); ++i) {
        initial_sand_pile[i] = new uint64_t[max_x - min_x + 1];
    }
    for (int i = 0; i < max_y - min_y + 1; ++i) {
        for (int j = 0; j < max_x - min_x + 1; ++j) {
            initial_sand_pile[i][j] = 0;
        }
    }
    while (input_file >> x >> y >> sand_number) {
        initial_sand_pile[y - min_y][x - min_x] = sand_number;
    } 

    input_file.close();
    result.sand_pile = initial_sand_pile;
    result.lines = max_y - min_y + 1;
    result.columns = max_x - min_x + 1;
    return result; 
}


void PileFall(InitialPile result, uint64_t max_iter, uint64_t freq, char output_name[]) {
    uint64_t** current_sand_pile = new uint64_t*[result.lines * 2];
    for (int i = 0; i < (result.lines * 2); ++i) {
        current_sand_pile[i] = new uint64_t[result.columns * 2];
    }

    for (int i = 0; i < result.lines * 2; ++i) {
        for (int j = 0; j < result.columns * 2; ++j) {
            current_sand_pile[i][j] = 0;
        }
    }

    for (int i = 0; i < result.lines; ++i) {
        for (int j = 0; j < result.columns; ++j) {
            current_sand_pile[i + (result.lines / 2)][j + (result.columns / 2)] = result.sand_pile[i][j];
        }
    }

    for (int i = 0; i < result.lines; ++i) {
        delete[] result.sand_pile[i]; 
    }
    delete[] result.sand_pile;
 

    uint16_t size_x = result.columns * 2;
    uint16_t size_y = result.lines * 2;

    int16_t min_x = result.columns / 2;
    int16_t max_x = result.columns + (result.columns / 2) - 1;
    int16_t min_y = result.lines / 2;
    int16_t max_y = result.lines + (result.lines / 2) - 1;
    bool is_stable = false;
    char new_output_name[256];
    uint64_t iter_count = 0;

    while (!is_stable && (iter_count < max_iter)) {
        ++iter_count;
        if (!min_x) {
            uint64_t** new_sand_pile = new uint64_t*[size_y];
            for (int i = 0; i < size_y; ++i) {
                new_sand_pile[i] = new uint64_t[size_x * 2];
            }
            for (int i = 0; i < size_y; ++i) {
                for (int j = 0; j < size_x * 2; ++j) {
                new_sand_pile[i][j] = 0; 
                }
            }
            for (int i = 0; i < size_y ; ++i) {
                for (int j = 0; j < size_x; ++j) {
                new_sand_pile[i][j + size_x] = current_sand_pile[i][j];
                }
            }
            for (int i = 0; i < size_y; ++i) {
                delete[] current_sand_pile[i]; 
            }
            delete[] current_sand_pile;

            current_sand_pile = new_sand_pile;
            min_x = size_x;
            max_x += size_x;
            size_x = size_x * 2;
        }

        if (max_x == size_x - 1) {
            uint64_t** new_sand_pile = new uint64_t*[size_y];
            for (int i = 0; i < size_y; ++i) {
                new_sand_pile[i] = new uint64_t[size_x * 2];
            }
            for (int i = 0; i < size_y; ++i) {
                for (int j = 0; j < size_x * 2; ++j) {
                    new_sand_pile[i][j] = 0; 
                }
            }
            for (int i = 0; i < size_y; ++i) {
                for (int j = 0; j < size_x; ++j) {
                new_sand_pile[i][j] = current_sand_pile[i][j];
                }
            }
            for (int i = 0; i < size_y; ++i) {
                delete[] current_sand_pile[i]; 
            }
            delete[] current_sand_pile;
            current_sand_pile = new_sand_pile;
            size_x = size_x * 2;
        }

        if (!min_y) {
            uint64_t** new_sand_pile = new uint64_t*[size_y * 2];
            for (int i = 0; i < size_y * 2; ++i) {
                new_sand_pile[i] = new uint64_t[size_x];
            }
            for (int i = 0; i < size_y * 2; ++i) {
                for (int j = 0; j < size_x; ++j) {
                    new_sand_pile[i][j] = 0; 
                }
            }
            for (int i = 0; i < size_y; ++i) {
                for (int j = 0; j < size_x; ++j) {
                new_sand_pile[i + size_y][j] = current_sand_pile[i][j];
                }
            }
            for (int i = 0; i < size_y; ++i) {
                delete[] current_sand_pile[i]; 
            }
            delete[] current_sand_pile;
            current_sand_pile = new_sand_pile;
            min_y = size_y;
            max_y += size_y;
            size_y = size_y * 2;
        }
 
        if (max_y == size_y - 1) {
            uint64_t** new_sand_pile = new uint64_t*[size_y * 2];
            for (int i = 0; i < size_y * 2; ++i) {
                new_sand_pile[i] = new uint64_t[size_x];
            }
            for (int i = 0; i < size_y * 2; ++i) {
                for (int j = 0; j < size_x; ++j) {
                    new_sand_pile[i][j] = 0; 
                }
            }
            for (int i = 0; i < size_y; ++i) {
                for (int j = 0; j < size_x; ++j) {
                new_sand_pile[i][j] = current_sand_pile[i][j];
                }
            }
            for (int i = 0; i < size_y; ++i) {
                delete[] current_sand_pile[i]; 
            }
            delete[] current_sand_pile;
            current_sand_pile = new_sand_pile;
            size_y = size_y * 2;
        }

        is_stable = true;
        for (int i = min_y; i <= max_y; ++i) {
            for (int j = min_x; j <= max_x; ++j) {
                if (current_sand_pile[i][j] >= 4) {
                    is_stable = false;
                    current_sand_pile[i][j] -= 4;
                    ++current_sand_pile[i - 1][j];
                    ++current_sand_pile[i + 1][j];
                    ++current_sand_pile[i][j - 1];
                    ++current_sand_pile[i][j + 1];
                    if (i - 1 < min_y) {
                        --min_y;
                    }
                      if (i + 1 > max_y) {
                        ++max_y;
                    }
                    if (j + 1 > max_x) {
                        ++max_x;
                    }
                     if (j - 1 < min_x) {
                        --min_x;
                    }
                }
            }
        }
        if(freq && ((iter_count % freq) == 0)) {
            strcpy(new_output_name, output_name);
            sprintf(new_output_name + strlen(new_output_name), "_%d", iter_count);
            CreatBmp(new_output_name, current_sand_pile, min_x, max_x, min_y, max_y);
        }
    }
    
    CreatBmp(output_name, current_sand_pile, min_x, max_x, min_y, max_y);
    for (int i = 0; i < size_y; ++i) {
        delete[] current_sand_pile[i]; 
    }
    delete[] current_sand_pile;
}