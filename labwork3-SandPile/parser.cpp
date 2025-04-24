#include "parser.h"

Arguments Parser(int argc, char** argv) {
    Arguments result;
  
    for (int i = 1; i < argc; ++i) {
        if ((strncmp(argv[i], "-i", sizeof("-i") - 1) == 0) || (strncmp(argv[i], "--input",  sizeof("--input") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            strncpy(result.input_name, argv[++i], sizeof(result.input_name) - 1);
        } else if ((strncmp(argv[i], "-o", sizeof("-o") - 1) == 0) || (strncmp(argv[i], "--output",  sizeof("--output") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            strncpy(result.output_name, argv[++i], sizeof(result.output_name) - 1);
        } else if ((strncmp(argv[i], "-m", sizeof("-m") - 1) == 0) || (strncmp(argv[i], "--max-iter",  sizeof("--max-iter") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            result.max_iter = std::strtoull(argv[++i], nullptr, 10);
        }
        else if ((strncmp(argv[i], "-f", sizeof("-f") - 1) == 0) || (strncmp(argv[i], "--freq",  sizeof("--freq") - 1) == 0)) {
            if (i + 1 >= argc) {
                result.input_error = true;
                break;
            }
            result.freq = std::strtoull(argv[++i], nullptr, 10);
        }
    }
    return result;
}
