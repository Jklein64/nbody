#include <glm/geometric.hpp>

#include "nbody.h"
#include "quadtree.h"

namespace nbody {

const float G = 6.67430e-11f;
const float GRID_SCALE = 2.9919574140000e13f;

void bbox(const Particles& particles, glm::vec2* a, glm::vec2* b) {
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

    *a = glm::vec2(x_min, y_min);
    *b = glm::vec2(x_max, y_max);
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
    for (size_t i = 0; i < params.particle_count; ++i) {
        // compute force that this particle receives
        glm::vec2 accel = glm::vec2();
        if (params.method == Method::kNaive) {
            accel = CalcAccelNaive(i);
        } else {
            accel = CalcAccelBarnesHut(i);
        }

        particles.accel[i] = accel;
        // in theory, leapfrog integration would go here
    }
}

glm::vec2 NBodySim::CalcAccelNaive(size_t i) {
    glm::vec2 accel = glm::vec2();
    for (size_t j = 0; j < params.particle_count; ++j) {
        if (j == i) continue;
        auto p1 = particles.pos[i];
        auto p2 = particles.pos[j];
        float r_squared = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
        accel += (G * particles.mass[j] / r_squared) * glm::normalize(p2 - p1);
    }

    return accel;
}

glm::vec2 NBodySim::CalcAccelBarnesHut(size_t i) {
    glm::vec2 accel;

    // bounding box coords (origin is in the top left)
    glm::vec2 a, b;
    bbox(particles, &a, &b);

    // initialize grid based on particles
    grid.Configure(GRID_SCALE, a, b);
    for (size_t i = 0; i < params.particle_count; i++) {
        auto idx = grid.Snap(particles.pos[i]);
        grid.Set(idx, grid.Get(idx) + particles.mass[i]);
    }

    // build quadtree on grid
    QuadTree tree(grid);
    tree.Build(particles.pos, particles.mass);

    // use quadtree to compute force
    const float theta = 0.5f; // Barnes-Hut opening parameter
    accel = tree.CalcAccel(i, particles.pos[i], particles.mass[i], theta);

    return accel;
}

void NBodySim::Save() { save_handler(particles, grid); }

void NBodySim::RegisterSaveHandler(SaveHandler handler) { save_handler = handler; }

}  // namespace nbody