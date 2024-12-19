#include "nbody.h"

#include <glm/geometric.hpp>

#include "quadtree.hpp"

namespace nbody {

// const float GRID_SCALE = 1e8f;
const float GRID_SCALE = 6e9f;

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
    Quadtree tree(a.x, a.y, b.x - a.x, b.y - a.y, 0.5);
    // add the bodies
    std::vector<std::shared_ptr<Body>> bodies;
    for (int i = 0; i < params.particle_count; ++i) {
        auto b = std::make_shared<Body>(particles.mass[i], particles.pos[i].x,
                                        particles.pos[i].y);
        bodies.push_back(b);
        tree.insertBody(*b, tree.root);
    }
    tree.calculateAllAccelerationsFromNode(tree.root);
    for (auto b : bodies) {
        printf("%f", b->xAcc);
    }
}

void NBodySim::Save() { save_handler(particles, grid); }

void NBodySim::RegisterSaveHandler(SaveHandler handler) { save_handler = handler; }

}  // namespace nbody