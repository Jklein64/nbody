#include "main.h"

#include <stdio.h>

#include <argparse/argparse.hpp>
#include <gif/gif.hpp>
#include <glm/exponential.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <random>

#include "nbody.h"

float color_scale;

const int GIF_DELAY = 10;

/**
 * @brief Generates the color corresponding to the given value of t in the inferno
 * colormap (from matplotlib). t should be greater than zero, but can be arbitrarily
 * large.
 *
 * @param t
 * @return glm::vec3
 */
glm::vec3 inferno(float t) {
    // https://www.shadertoy.com/view/3lBXR3
    auto c0 = glm::vec3(0.00021894037, 0.0016510046, -0.019480899);
    auto c1 = glm::vec3(0.10651341949, 0.5639564368, 3.9327123889);
    auto c2 = glm::vec3(11.6024930825, -3.972853966, -15.94239411);
    auto c3 = glm::vec3(-41.703996131, 17.436398882, 44.354145199);
    auto c4 = glm::vec3(77.1629356994, -33.40235894, -81.80730926);
    auto c5 = glm::vec3(-71.319428245, 32.626064264, 73.209519858);
    auto c6 = glm::vec3(25.1311262248, -12.24266895, -23.07032500);
    return c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
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
    program.add_argument("-c", "--color-scale").scan<'g', float>().default_value(3e3f);

    try {
        program.parse_args(argc, argv);
        color_scale = program.get<float>("-c");
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

    GifWriter gif_writer;
    GifBegin(&gif_writer, "out.gif", params.grid_width, params.grid_height, GIF_DELAY);
    sim.RegisterSaveHandler([&gif_writer, &params](auto densities) {
        std::vector<uint8_t> colors(densities.size() * 4, 0);
        for (size_t i = 0; i < densities.size(); i++) {
            // scale to be in [0, 1]
            float t = 1.0f - glm::exp(-densities[i] * color_scale);
            glm::vec3 color = glm::clamp(inferno(t), 0.0f, 1.0f);
            colors[i * 4 + 0] = static_cast<uint8_t>(color.x * 255.0f);
            colors[i * 4 + 1] = static_cast<uint8_t>(color.y * 255.0f);
            colors[i * 4 + 2] = static_cast<uint8_t>(color.z * 255.0f);
            colors[i * 4 + 3] = 255;
        }
        GifWriteFrame(&gif_writer, colors.data(), params.grid_width, params.grid_height,
                      GIF_DELAY);
    });

    for (size_t i = 0; i < params.frame_count; i++) {
        sim.Step();
        // save every save_interval frames
        if (i % params.save_interval == 0) {
            sim.Save();
        }
    }

    GifEnd(&gif_writer);

    return 0;
}