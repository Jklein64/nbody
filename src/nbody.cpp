#include "nbody.h"

#include <stdio.h>

#include "argparse/argparse.hpp"
#include "gif.h"

int main(int argc, char *argv[]) {
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
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::vector<uint8_t> black(w * h * 4, 0);
    std::vector<uint8_t> white(w * h * 4, 255);
    auto fileName = "bwgif.gif";
    int delay = 100;
    GifWriter g;
    GifBegin(&g, fileName, w, h, delay);
    GifWriteFrame(&g, black.data(), w, h, delay);
    GifWriteFrame(&g, white.data(), w, h, delay);
    GifEnd(&g);

    printf("Hello! Grid is %lux%lu\n", w, h);
    return 0;
}