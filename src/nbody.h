#pragma once

#include <stddef.h>

#include <glm/vec2.hpp>
#include <vector>

namespace nbody {

struct SimParams {
    // width and height of simulation grid. particles have floating-point
    // locations that get truncated to grid cells when determining updates
    size_t grid_width, grid_height;

    // the number of particles to simulate
    size_t particle_count;

    // the number of iterations to run the simulation
    size_t frame_count;

    // every save_interval iterations, a frame of the output gif is written. if
    // save_interval < 0, then no output gif is written.
    int save_interval;
};

struct SimState {
    // this will need to change for Barnes-Hut
    std::vector<glm::vec2> particles;
};

void init(const SimParams& params, glm::vec2 (*sampler)());
void step(const SimParams& params, const SimState& state);
void free(const SimState& state);

}  // namespace nbody
