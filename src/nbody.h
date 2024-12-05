#pragma once

#include <stddef.h>

#include <functional>
#include <glm/vec2.hpp>
#include <vector>

#include "grid.h"

namespace nbody {

typedef struct Particle {
    float mass;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 acceleration;
} Particle;

struct SimParams {
    // number of rows and columns of the simulation grid. particles have floating-point
    // locations that get truncated to grid cells when determining updates
    size_t grid_width, grid_height;

    // correspondence between cells and physical units (meters)
    float cell_width = 1.0f;
    float cell_height = 1.0f;

    // the number of particles to simulate
    size_t particle_count;

    // the number of iterations to run the simulation
    size_t frame_count;

    // every save_interval iterations, a frame of the output gif is written. if
    // save_interval < 0, then no output gif is written.
    int save_interval;
};

class NBodySim {
   public:
    const SimParams& params;
    NBodySim(const SimParams& params, const std::function<glm::vec2()>& sampler);
    void Step();
    void Save();
    void RegisterSaveHandler(std::function<void(const std::vector<float>&)> handler);

   private:
    // this will need to change for Barnes-Hut
    std::vector<Particle> particles;
    grid::Grid grid;
    size_t frame = 0;
    std::function<void(const std::vector<float>&)> save_handler;
};

}  // namespace nbody
