#pragma once

#include <stddef.h>

#include <cstdlib>
#include <functional>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

#include "grid.h"

namespace nbody {

typedef struct Particles {
    std::vector<float> mass;
    std::vector<glm::vec2> pos;
    std::vector<glm::vec2> accel;
} Particles;

enum class Method { kNaive, kBarnesHut };

struct SimParams {
    // number of rows and columns of the simulation grid. particles have floating-point
    // locations that get truncated to grid cells when determining updates
    size_t grid_width, grid_height;

    // correspondence between cells and physical units (meters)
    float cell_width = 1.0f;
    float cell_height = 1.0f;

    // the number of particles to simulate
    size_t particle_count;

    // the algorithm to use for steps
    Method method;
};

typedef std::function<void(const Particles&, const nbody::Grid&)> SaveHandler;

class NBodySim {
   public:
    const SimParams& params;
    NBodySim(const SimParams& params,
             const std::function<std::pair<glm::vec2, float>()>& sampler);
    void Step();
    void Save();
    glm::vec2 CalcAccelNaive(size_t i);
    glm::vec2 CalcAccelBarnesHut(size_t i);
    void RegisterSaveHandler(SaveHandler handler);

   private:
    Particles particles;
    nbody::Grid grid;

    SaveHandler save_handler;
};

}  // namespace nbody

inline std::string to_string(nbody::Method m) {
    switch (m) {
        case nbody::Method::kNaive:
            return "naive";
        case nbody::Method::kBarnesHut:
            return "barnes-hut";
        default:
            // unreachable
            exit(EXIT_FAILURE);
    }
}