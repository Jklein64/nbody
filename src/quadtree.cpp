#include "quadtree.h"

#include <stddef.h>

#include <cstdlib>
#include <glm/vec2.hpp>
#include <memory>

#include "glm/common.hpp"

namespace nbody {
Quadtree::Quadtree(float theta, const nbody::Particles& particles)
    : theta(theta), particles(particles) {
    // build the quadtree
    root = std::make_shared<Node>();
    root->com = glm::vec2(0);
    root->com_n = 0;
    root->bodies = {-1, -1, -1, -1};
    root->quadrants = {nullptr, nullptr, nullptr, nullptr};
    root->mass = 0.0f;
    // get the center location and size
    auto [a, b] = bbox(particles);
    root->size = glm::max(b.x - a.x, b.y - a.y);
    root->center = (a + b) / 2.0f;

    for (size_t i = 0; i < particles.pos.size(); ++i) {
        std::shared_ptr<Node> x = root;
        int q = CalcQuadrant(*x, particles.pos[i]);
        while (x->quadrants[q] != nullptr) {
            float m = particles.mass[i];
            glm::vec2 pos = particles.pos[i];
            x->mass += m;
            // cumulative average
            x->com = ((m * pos) + (float)x->com_n * x->com) / (float)(x->com_n + 1);
            x->com_n++;
        }
        if (x->bodies[q] < 0) {
            // nothing is there, so add this point
            x->bodies[q] = i;
        } else {
            // something is there, so we need to split the quadrant
        }
    }

    if (root->bodies.size() == 0) {
        root->bodies.push_back(i);
        root->mass = particles.mass[i];
        root->com = particles.pos[i];
    } else if (root->children.size() > 0) {
        // x is an internal node
    }
}
}

// 1 | 0
// --+--
// 2 | 3
int Quadtree::CalcQuadrant(const Node& node, const glm::vec2& pos) const {
    if (pos.x < node.center.x) {
        if (pos.y < node.center.y) {
            return 2;
        } else {
            return 1;
        }
    } else {
        if (pos.y < node.center.y) {
            return 3;
        } else {
            return 0;
        }
    }
}

bool Quadtree::MAC(const glm::vec2& pos, const Node& n) const {
    return NodeSize(n) / (pos - n.com).length() < theta;
}

int Quadtree::NodeLevel(const Node& node) const { return 0; }

float Quadtree::NodeSize(const Node& node) const { return 0.0; }

Node& Quadtree::NodeParent(const Node& node) const {}

Node& Quadtree::NodeChild(const Node& node, int i) const {}

glm::vec2 Quadtree::CalcAccel(size_t i) const { return glm::vec2(); }

}  // namespace nbody