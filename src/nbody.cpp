#include "nbody.h"

#include <glm/geometric.hpp>

#include "quadtree.h"

namespace nbody {

// const float GRID_SCALE = 1e8f;
const float GRID_SCALE = 6e9f;

std::pair<glm::vec2, glm::vec2> bbox(const Particles& particles) {
    float x_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::min();
    for (auto& pos : particles.pos) {
        x_min = std::min(x_min, pos.x);
        x_max = std::max(x_max, pos.x);
        y_min = std::min(y_min, pos.y);
        y_max = std::max(y_max, pos.y);
    }

    return std::make_pair(glm::vec2(x_min, y_min), glm::vec2(x_max, y_max));
}

NBodySim::NBodySim(const SimParams& params,
                   const std::function<std::pair<glm::vec2, float>()>& sampler)
    : params(params) {
    // initialize particles
    particles.pos.reserve(params.particle_count);
    particles.mass.reserve(params.particle_count);
    particles.accel.resize(params.particle_count);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto [pos, mass] = sampler();
        particles.pos.push_back(pos);
        particles.mass.push_back(mass);
    }
}

void NBodySim::Step() {
    if (params.method == Method::kNaive) {
        CalcAccelNaive();
    } else {
        CalcAccelBarnesHut();
    }

    // in theory, leapfrog integration would go here
}

void NBodySim::CalcAccelNaive() {
    for (size_t i = 0; i < params.particle_count; ++i) {
        // compute force that this particle receives
        glm::vec2 accel = glm::vec2();
        for (size_t j = 0; j < params.particle_count; ++j) {
            if (j == i) continue;
            auto p1 = particles.pos[i];
            auto p2 = particles.pos[j];
            float r_squared =
                (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
            accel += (G * particles.mass[j] / r_squared) * glm::normalize(p2 - p1);
        }

        particles.accel[i] = accel;
    }
}

void NBodySim::CalcAccelBarnesHut() {
    // bounding box coords (origin is in the top left)
    auto [a, b] = bbox(particles);

    // initialize grid based on particles
    grid.Configure(GRID_SCALE, a, b);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto idx = grid.Snap(particles.pos[i]);
        grid.Set(idx, grid.Get(idx) + particles.mass[i]);
    }

    Quadtree tree(0.5f, grid, particles);
    for (size_t i = 0; i < params.particle_count; ++i) {
        // compute force that this particle receives
        glm::vec2 accel = tree.CalcAccel(i);
        // particles.accel[i] = accel;
    }
}

void NBodySim::Save() { save_handler(particles, grid); }

void NBodySim::RegisterSaveHandler(SaveHandler handler) { save_handler = handler; }

}  // namespace nbody