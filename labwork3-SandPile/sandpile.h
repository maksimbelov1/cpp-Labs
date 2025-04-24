#include <fstream>
#include <cstdint>
#include <iostream>
#include <cstring>

struct InitialPile {
    uint64_t** sand_pile;
    uint16_t lines;
    uint16_t columns;    
};

InitialPile ReadingTsv(char input_name[]);
InitialPile ReadingBmp(char input_name[]);
void PileFall(InitialPile result, uint64_t max_iter, uint64_t freq, char output_name[]);