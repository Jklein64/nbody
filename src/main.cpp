#include "main.h"

#include <stdio.h>

#include "argparse/argparse.hpp"
#include "gif.h"
#include "nbody.h"

int main(int argc, char *argv[]) {
    nbody::SimParams params;
    argparse::ArgumentParser program("nbody");
    program.add_argument("-w", "--grid-width")
        .scan<'i', size_t>()
        .default_value<size_t>(100)
        .store_into(params.grid_width)
        .help("Number of horizontal grid cells.");
    program.add_argument("-h", "--grid-height")
        .scan<'i', size_t>()
        .default_value<size_t>(100)
        .store_into(params.grid_height)
        .help("Number of vertical grid cells.");
    program.add_argument("--particle-count")
        .scan<'i', size_t>()
        .default_value<size_t>(10)
        .store_into(params.particle_count)
        .help("The number of particles to simulate.");
    program.add_argument("--frame-count")
        .scan<'i', size_t>()
        .default_value<size_t>(1000)
        .store_into(params.frame_count)
        .help("The number of frames to simulate.");
    program.add_argument("--save-interval")
        .scan<'i', int>()
        .default_value(-1)
        .store_into(params.save_interval)
        .help("Number of frames between saves. Negative never saves");

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    std::vector<uint8_t> black(params.grid_width * params.grid_height * 4, 0);
    std::vector<uint8_t> white(params.grid_width * params.grid_height * 4, 255);
    auto fileName = "bwgif.gif";
    int delay = 100;
    GifWriter g;
    GifBegin(&g, fileName, params.grid_width, params.grid_height, delay);
    GifWriteFrame(&g, black.data(), params.grid_width, params.grid_height,
                  delay);
    GifWriteFrame(&g, white.data(), params.grid_width, params.grid_height,
                  delay);
    GifEnd(&g);

    printf("Hello! Grid is %lux%lu\n", params.grid_width, params.grid_height);
    return 0;
}