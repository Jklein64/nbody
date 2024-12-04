#include "nbody.h"
#include <stdio.h>
#include <argparse/argparse.hpp>

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("nbody");
    size_t w, h;
    program.add_argument("-w", "--grid_width")
        .scan<'i', size_t>()
        .required()
        .store_into(w)
        .help("Number of horizontal grid cells.");
    program.add_argument("-h", "--grid_height")
        .scan<'i', size_t>()
        .required()
        .store_into(h)
        .help("Number of vertical grid cells.");
    
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    printf("Hello! Grid is %lux%lu\n", w, h);
    return 0;
}