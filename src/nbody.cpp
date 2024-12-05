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
        particles.push_back(sampler());
    }

    // initialize grid based on particles
    for (size_t i = 0; i < params.particle_count; i++) {
        auto idx = grid.snap(particles[i].x, particles[i].y);
        grid.set(idx, grid.get(idx) + 1.0f);
    }
}

void NBodySim::Step() { frame++; }

void NBodySim::Save() { save_handler(grid.ViewFlattened()); }

void NBodySim::RegisterSaveHandler(
    std::function<void(const std::vector<float>&)> handler) {
    save_handler = handler;
}

}  // namespace nbody