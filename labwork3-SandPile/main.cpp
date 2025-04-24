#include "parser.h"
#include "bmp.h"
#include "sandpile.h"
#include <iostream>


int main(int argc, char* argv[]) {
    Arguments parsed_args = Parser(argc, argv);
    if (parsed_args.input_error) {
        std:: cerr << "invalid input data";
    } else {
        // case 1: input - bmp
        InitialPile pile = ReadingBmp(parsed_args.input_name);
        PileFall(pile, parsed_args.max_iter, parsed_args.freq, parsed_args.output_name);

        // case 2: input - tsv (for example "1 1 239239")
        // InitialPile pile = ReadingTsv(parsed_args.input_name);
        // PileFall(pile, parsed_args.max_iter, parsed_args.freq, parsed_args.output_name);
    }
}