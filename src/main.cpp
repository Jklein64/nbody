#include <stdio.h>

#include <argparse/argparse.hpp>
#include <cstdlib>
#include <fstream>
#include <glm/exponential.hpp>
#include <glm/ext/scalar_common.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>
#include <random>

#include "grid.h"
#include "nbody.h"

class ParticleGenerator {
    // see https://en.cppreference.com/w/cpp/numeric/random/normal_distribution
    std::random_device rd{};
    std::mt19937 gen{rd()};

    std::uniform_real_distribution<float> uniform_r;
    std::uniform_real_distribution<float> uniform_theta;
    std::uniform_real_distribution<float> uniform_mass;

   public:
    ParticleGenerator(const nbody::SimParams &params)
        : uniform_r(0, 4e10),
          uniform_theta(0, 2 * glm::pi<float>()),
          uniform_mass(1e21, 1e30) {}

    std::pair<glm::vec2, float> sample() {
        float r = uniform_r(gen);
        float theta = uniform_theta(gen);
        float mass = uniform_mass(gen);
        glm::vec2 pos = glm::vec2(r * glm::cos(theta), r * glm::sin(theta));
        return std::make_pair(pos, mass);
    };
};

int main(int argc, char *argv[]) {
    nbody::SimParams params;
    int num_trials = 0;
    std::string method_string;
    argparse::ArgumentParser program("nbody");
    program.add_argument("-m", "--method")
        .default_value("naive")
        .store_into(method_string)
        .help("Method to use, either naive or barnes-hut.");
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
    program.add_argument("--num_trials")
        .scan<'i', int>()
        .default_value(1)
        .store_into(num_trials)
        .help("The number of trials to run to determine runtime.");

    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(EXIT_FAILURE);
    }

    // handle method string
    if (method_string == "naive") {
        params.method = nbody::Method::kNaive;
    } else if (method_string == "barnes-hut") {
        params.method = nbody::Method::kBarnesHut;
    } else {
        fprintf(stderr, "Unknown method %s", method_string.c_str());
        exit(EXIT_FAILURE);
    }

    auto gen = ParticleGenerator(params);
    nbody::NBodySim sim(params, std::bind(&ParticleGenerator::sample, &gen));

    int trial_number = 0;
    sim.RegisterSaveHandler(
        [&params, &trial_number](nbody::Particles particles, nbody::Grid grid) {
            // create filename with date, time, and trial number
            auto time = std::time(nullptr);
            std::stringstream ss;
            ss << "out/";
            // see https://stackoverflow.com/a/45419863
            ss << std::put_time(std::gmtime(&time), "%F %T") << " " << trial_number;
            ss << ".out";
            std::string filename = ss.str();

            // create file and save data
            printf("saving to file at \"%s\"\n", filename.c_str());
            std::ofstream outfile;
            outfile.open(filename);
            outfile << to_string(params.method) << std::endl;
            outfile << params.particle_count << std::endl;
            outfile << grid.x << " ";
            outfile << grid.y << " ";
            outfile << grid.width << " ";
            outfile << grid.height;
            outfile << std::endl;
            for (size_t i = 0; i < params.particle_count; ++i) {
                outfile << particles.pos[i].x << " ";
                outfile << particles.pos[i].y << " ";
                outfile << particles.accel[i].x << " ";
                outfile << particles.accel[i].y << " ";
                outfile << particles.mass[i] << " ";
                outfile << std::endl;
            }
            outfile.close();
        });

    // run timing experiment
    for (; trial_number < num_trials; trial_number++) {
        // TODO start timing infrastructure
        sim.Step();
        // TODO stop timing infrastructure
        sim.Save();
    }

    return 0;
}