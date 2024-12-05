#include "nbody.h"

#include <glm/geometric.hpp>

namespace nbody {

const float DELTA_TIME = 1.0f;  // / 20.0f;
const float G = 1e14f;

void bbox(const std::vector<Particle>& particles, float* ax, float* ay, float* bx,
          float* by) {
    float x_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::min();
    for (auto& p : particles) {
        x_min = std::min(x_min, p.position.x);
        x_max = std::max(x_max, p.position.x);
        y_min = std::min(y_min, p.position.y);
        y_max = std::max(y_max, p.position.y);
    }
    *ax = x_min;
    *ay = y_min;
    *bx = x_max;
    *by = y_max;
}

NBodySim::NBodySim(const SimParams& params, const std::function<glm::vec2()>& sampler)
    : params(params) {
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

    // bounding box coords (origin is in the top left)
    float ax, ay, bx, by;
    bbox(particles, &ax, &ay, &bx, &by);

    // initialize grid based on particles
    grid.Configure(ax, ay, bx, by);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto p = particles[i];
        auto idx = grid.Snap(p.position.x, p.position.y);
        grid.Set(idx, grid.Get(idx) + p.mass);
    }
}

void NBodySim::Step() {
    // update grid
    float ax, ay, bx, by;
    bbox(particles, &ax, &ay, &bx, &by);
    grid.Configure(ax, ay, bx, by);
    for (size_t i = 0; i < params.particle_count; i++) {
        Particle p = particles[i];
        auto idx = grid.Snap(p.position.x, p.position.y);
        grid.Set(idx, grid.Get(idx) + p.mass);
    }

    for (auto& p : particles) {
        // leapfrog integration https://en.wikipedia.org/wiki/Leapfrog_integration
    }
}

void NBodySim::Save() { save_handler(particles, grid); }

void NBodySim::RegisterSaveHandler(SaveHandler handler) { save_handler = handler; }

}  // namespace nbody