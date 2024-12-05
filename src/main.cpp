#include "main.h"

#include <stdio.h>

#include <argparse/argparse.hpp>
#include <gif/gif.hpp>
#include <glm/exponential.hpp>
#include <glm/vec3.hpp>
#include <random>

#include "nbody.h"

const float COLOR_SCALE = 5;

/**
 * @brief Generates the color corresponding to the given value of t in the inferno
 * colormap (from matplotlib). t should be greater than zero, but can be arbitrarily
 * large.
 *
 * @param t
 * @return glm::vec3
 */
glm::vec3 inferno(float t) {
    // scale t to be in [0, 1]
    t = 1.0f - glm::exp(-t * COLOR_SCALE);
    // https://www.shadertoy.com/view/3lBXR3
    auto c0 = glm::vec3(0.00021894037, 0.0016510046, -0.019480899);
    auto c1 = glm::vec3(0.10651341949, 0.5639564368, 3.9327123889);
    auto c2 = glm::vec3(11.6024930825, -3.972853966, -15.94239411);
    auto c3 = glm::vec3(-41.703996131, 17.436398882, 44.354145199);
    auto c4 = glm::vec3(77.1629356994, -33.40235894, -81.80730926);
    auto c5 = glm::vec3(-71.319428245, 32.626064264, 73.209519858);
    auto c6 = glm::vec3(25.1311262248, -12.24266895, -23.07032500);
    // inline sRGB_to_linear from https://compute.toys/view/683
    auto color = c0 + t * (c1 + t * (c2 + t * (c3 + t * (c4 + t * (c5 + t * c6)))));
    auto higher = glm::pow((color + 0.055f) / 1.055f, glm::vec3(2.4));
    auto lower = color / 12.92f;
    return glm::vec3(color.x > 0.04045 ? higher.x : lower.x,
                     color.x > 0.04045 ? higher.y : lower.y,
                     color.x > 0.04045 ? higher.z : lower.z);
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

    // see https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution dist_x{
        static_cast<float>(params.grid_width) * params.cell_width / 2,
        20.0f * params.cell_width};
    std::normal_distribution dist_y{
        static_cast<float>(params.grid_height) * params.cell_height / 2,
        20.0f * params.cell_height};

    nbody::NBodySim sim(params, [&dist_x, &dist_y, &gen, &params] {
        float x = std::clamp(dist_x(gen), 0.0f,
                             static_cast<float>(params.grid_width) * params.cell_width);
        float y = std::clamp(dist_y(gen), 0.0f,
                             static_cast<float>(params.grid_height) * params.cell_height);
        return glm::vec2(x, y);
    });

    GifWriter gif_writer;
    GifBegin(&gif_writer, "out.gif", params.grid_width, params.grid_height, 100);
    sim.RegisterSaveHandler([&gif_writer, &params](auto floats) {
        std::vector<uint8_t> colors(floats.size() * 4);
        for (size_t i = 0; i < floats.size(); i++) {
            glm::vec3 color = inferno(floats[i]);
            colors.push_back(static_cast<uint8_t>(
                floats[i]));  // static_cast<uint8_t>(color.x * 255.f);
            colors.push_back(static_cast<uint8_t>(
                floats[i]));  // static_cast<uint8_t>(color.y * 255.f);
            colors.push_back(static_cast<uint8_t>(
                floats[i]));  // static_cast<uint8_t>(color.z * 255.f);
            colors.push_back(static_cast<uint8_t>(100));
        }
        GifWriteFrame(&gif_writer, colors.data(), params.grid_width, params.grid_height,
                      100);
        for (auto f : floats) {
            printf("%.1f, ", f);
        }
    });

    for (size_t i = 0; i < params.frame_count; i++) {
        sim.Step();
        // save every save_interval frames
        if (i % params.save_interval == 0) {
            sim.Save();
        }
    }

    // GifEnd(&gif_writer);

    // std::vector<uint8_t> black(params.grid_width * params.grid_height * 4, 0);
    // std::vector<uint8_t> white(params.grid_width * params.grid_height * 4, 255);
    // auto fileName = "bwgif.gif";
    // int delay = 100;
    // GifWriteFrame(&gif_writer, black.data(), params.grid_width, params.grid_height,
    // 100); GifWriteFrame(&gif_writer, white.data(), params.grid_width,
    // params.grid_height, 100);
    GifEnd(&gif_writer);

    printf("Hello! Grid is %lux%lu\n", params.grid_width, params.grid_height);
    return 0;
}