#include "nbody.h"

#include <glm/geometric.hpp>

namespace nbody {

const float DELTA_TIME = 1.0f;  // / 20.0f;
const float G = 1e14f;

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
        auto idx = grid.Snap(p.position.x, p.position.y);
        grid.Set(idx, grid.Get(idx) + p.mass);
    }
}

void NBodySim::Step() {
    for (auto& p : particles) {
        // p.velocity = glm::vec2(10, 1);
        p.velocity += p.acceleration * DELTA_TIME / 2.0f;
        p.position += p.velocity * DELTA_TIME;
        p.acceleration = glm::vec2(0, 0);
        for (auto& q : particles) {
            if (&p == &q) continue;
            glm::vec2 r = q.position - p.position;
            p.acceleration += 0.01f * glm::normalize(r) / glm::dot(r, r);
        }
        p.velocity += p.acceleration * DELTA_TIME / 2.0f;
    }
    // for (size_t i = 0; i < params.particle_count; i++) {
    //     Particle p = particles[i];

    //     // kick
    //     // p.velocity += p.acceleration * DELTA_TIME / 2.0f;

    //     // drift
    //     p.position += p.velocity * DELTA_TIME;

    //     // recompute new accelerations (naive)
    //     p.acceleration = glm::vec2(0);
    //     for (size_t j = 0; j < params.particle_count; j++) {
    //         if (j == i) continue;
    //         Particle q = particles[j];
    //         auto r = q.position - p.position;
    //         p.acceleration += G * p.mass * q.mass / glm::dot(r, r) * glm::normalize(r);
    //     }

    //     // kick
    //     p.velocity += p.acceleration * DELTA_TIME / 2.0f;
    // }

    // clear and update grid
    grid.Clear();
    for (size_t i = 0; i < params.particle_count; i++) {
        Particle p = particles[i];
        auto idx = grid.Snap(p.position.x, p.position.y);
        grid.Set(idx, grid.Get(idx) + p.mass);
    }

    frame++;
}

void NBodySim::Save() { save_handler(grid.ViewFlattened()); }

void NBodySim::RegisterSaveHandler(
    std::function<void(const std::vector<float>&)> handler) {
    save_handler = handler;
}

}  // namespace nbody