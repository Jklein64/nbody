#include <stdio.h>

#include <argparse/argparse.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <random>

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

    // see https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
    std::random_device rd{};
    std::mt19937 gen{rd()};
    auto center =
        glm::vec2(static_cast<float>(params.grid_width) * params.cell_width / 2,
                  static_cast<float>(params.grid_height) * params.cell_height / 2);
    std::uniform_real_distribution<float> dist_r(0, std::min(center.x, center.y));
    std::uniform_real_distribution<float> dist_th(0, 2 * glm::pi<float>());

    nbody::NBodySim sim(params, [&dist_r, &dist_th, &gen, &params, &center] {
        float r = dist_r(gen);
        float th = dist_th(gen);
        return center + glm::vec2(r * glm::cos(th), r * glm::sin(th));
    });

    sim.RegisterSaveHandler([&params](auto particles, auto grid) {
        /* TODO: do something with particle data, either SDL or write to text file */
    });

    for (size_t i = 0; i < params.frame_count; i++) {
        sim.Step();
        // save every save_interval frames
        if (i % params.save_interval == 0) {
            sim.Save();
        }
    }

    return 0;
}