#include "main.h"

#include <gif.h>
#include <stdio.h>

#include <argparse/argparse.hpp>
#include <glm/exponential.hpp>
#include <glm/vec3.hpp>

#include "nbody.h"

glm::vec3 sRGB_to_linear(glm::vec3 color) {
    // https://compute.toys/view/683
    auto higher = glm::pow((color + 0.055f) / 1.055f, glm::vec3(2.4));
    auto lower = color / 12.92f;
    return glm::vec3(color.x > 0.04045 ? higher.x : lower.x,
                     color.x > 0.04045 ? higher.y : lower.y,
                     color.x > 0.04045 ? higher.z : lower.z);
}

glm::vec3 inferno(float t) {
    // https://www.shadertoy.com/view/3lBXR3
    auto c0 = glm::vec3(0.00021894037, 0.0016510046, -0.019480899);
    auto c1 = glm::vec3(0.10651341949, 0.5639564368, 3.9327123889);
    auto c2 = glm::vec3(11.6024930825, -3.972853966, -15.94239411);
    auto c3 = glm::vec3(-41.703996131, 17.436398882, 44.354145199);
    auto c4 = glm::vec3(77.1629356994, -33.40235894, -81.80730926);
    auto c5 = glm::vec3(-71.319428245, 32.626064264, 73.209519858);
    auto c6 = glm::vec3(25.1311262248, -12.24266895, -23.07032500);
    return sRGB_to_linear(
        c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6))))));
}

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