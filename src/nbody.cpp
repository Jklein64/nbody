#include "nbody.h"

namespace nbody {

grid::Grid grid_from_params(const SimParams& params) {
    return std::move(grid::Grid{params.grid_width, params.grid_height, params.cell_width,
                                params.cell_height});
}

NBodySim::NBodySim(const SimParams& params, const std::function<glm::vec2()>& sampler)
    : params(params), grid(grid_from_params(params)) {
    // initialize particles
    particles.reserve(params.particle_count);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto x = sampler();
        particles.push_back((Particle){// initial conditions. these matter!
                                       .position = x,
                                       .velocity = glm::vec2(0),
                                       .acceleration = glm::vec2(0),
                                       .mass = 0.35f / params.particle_count});
    }

    // initialize grid based on particles
    for (size_t i = 0; i < params.particle_count; i++) {
        auto p = particles[i];
        auto idx = grid.snap(p.position.x, p.position.y);
        grid.set(idx, grid.get(idx) + p.mass);
    }
}

void NBodySim::Step() {
    if (frame == 0) {
        // need to kickstart the sim?

    } else {
    }

    frame++;
}

void NBodySim::Save() { save_handler(grid.ViewFlattened()); }

void NBodySim::RegisterSaveHandler(
    std::function<void(const std::vector<float>&)> handler) {
    save_handler = handler;
}

}  // namespace nbody