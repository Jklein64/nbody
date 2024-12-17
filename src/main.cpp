#include <stdio.h>

#include <argparse/argparse.hpp>
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
    glm::vec2 center;

    std::uniform_real_distribution<float> uniform_r;
    std::uniform_real_distribution<float> uniform_theta;
    std::uniform_real_distribution<float> uniform_mass;

    glm::vec2 calc_center(const nbody::SimParams &params) {
        return glm::vec2(static_cast<float>(params.grid_width) * params.cell_width / 2,
                         static_cast<float>(params.grid_height) * params.cell_height / 2);
    }

   public:
    ParticleGenerator(const nbody::SimParams &params)
        : center(calc_center(params)),
          uniform_r(0, std::min(center.x, center.y)),
          uniform_theta(0, 2 * glm::pi<float>()),
          uniform_mass(1e21, 1e30) {}

    nbody::Particle sample() {
        float r = uniform_r(gen);
        float theta = uniform_theta(gen);
        float mass = uniform_mass(gen);
        glm::vec2 pos = center + glm::vec2(r * glm::cos(theta), r * glm::sin(theta));
        return (nbody::Particle){.mass = mass, .pos = pos};
    };
};

int main(int argc, char *argv[]) {
    nbody::SimParams params;
    int num_trials = 0;
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
        std::exit(1);
    }

    auto gen = ParticleGenerator(params);
    nbody::NBodySim sim(params, std::bind(&ParticleGenerator::sample, &gen));

    int trial_number = 0;
    sim.RegisterSaveHandler(
        [&trial_number](std::vector<nbody::Particle> particles, nbody::Grid grid) {
            // create filename with date, time, and trial number
            auto time = std::time(nullptr);
            std::stringstream ss;
            ss << "out/";
            // see https://stackoverflow.com/a/45419863
            ss << std::put_time(std::gmtime(&time), "%F %T%z") << " " << trial_number;
            ss << ".out";
            std::string filename = ss.str();

            // create file and save data
            printf("saving to file at \"%s\"\n", filename.c_str());
            std::ofstream outfile;
            outfile.open(filename);
            outfile << particles.size() << std::endl;
            outfile << grid.x << " ";
            outfile << grid.y << " ";
            outfile << grid.width << " ";
            outfile << grid.height;
            outfile << std::endl;
            for (const auto &p : particles) {
                outfile << p.pos.x << " ";
                outfile << p.pos.y << " ";
                outfile << p.mass << " ";
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