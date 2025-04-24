#include <cstdint>
#include <string.h>
#include <cstdlib>

struct Arguments {
    char input_name[1024];
    bool input_error = false;
    char output_name[1024];
    uint64_t max_iter = 0;
    uint64_t freq = 0;
};

Arguments Parser(int argc, char** argv);