#include "nbody.h"

#include <glm/geometric.hpp>

namespace nbody {

const float G = 6.67430e-11f;
const float GRID_SCALE = 2.9919574140000e13f;

void bbox(const std::vector<Particle>& particles, glm::vec2* a, glm::vec2* b) {
    float x_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::min();
    for (auto& p : particles) {
        x_min = std::min(x_min, p.pos.x);
        x_max = std::max(x_max, p.pos.x);
        y_min = std::min(y_min, p.pos.y);
        y_max = std::max(y_max, p.pos.y);
    }

    *a = glm::vec2(x_min, y_min);
    *b = glm::vec2(x_max, y_max);
}

NBodySim::NBodySim(const SimParams& params, const std::function<Particle()>& sampler)
    : params(params) {
    // initialize particles
    particles.reserve(params.particle_count);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto p = sampler();
        particles.push_back(p);
    }
}

void NBodySim::Step() {
    for (size_t i = 0; i < particles.size(); ++i) {
        // compute force that this particle receives
        float f = 0.0f;
        if (params.method == Method::kNaive) {
            f = CalcForceNaive(i);
        } else {
            f = CalcForceBarnesHut(i);
        }

        // in theory, leapfrog integration would go here
    }
}

float NBodySim::CalcForceNaive(size_t i) {
    float force = 0.0f;
    for (size_t j = 0; j < particles.size(); ++j) {
        if (j == i) continue;
        auto p1 = particles[i];
        auto p2 = particles[j];
        float r_squared = (p1.pos.x - p2.pos.x) * (p1.pos.x - p2.pos.x) +
                          (p1.pos.y - p2.pos.y) * (p1.pos.y - p2.pos.y);
        force += G * p1.mass * p2.mass / r_squared;
    }

    return force;
}

float NBodySim::CalcForceBarnesHut(size_t i) {
    float force = 0.0f;

    // bounding box coords (origin is in the top left)
    glm::vec2 a, b;
    bbox(particles, &a, &b);

    // initialize grid based on particles
    grid.Configure(GRID_SCALE, a, b);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto p = particles[i];
        auto idx = grid.Snap(p.pos);
        grid.Set(idx, grid.Get(idx) + p.mass);
    }

    // TODO build quadtree on grid

    // TODO use quadtree to compute force

    return force;
}

void NBodySim::Save() { save_handler(particles, grid); }

void NBodySim::RegisterSaveHandler(SaveHandler handler) { save_handler = handler; }

}  // namespace nbody