#include "quadtree.h"

#include <stddef.h>

#include <cstdlib>
#include <glm/vec2.hpp>

namespace nbody {
Quadtree::Quadtree(float theta, const nbody::Grid& grid,
                   const nbody::Particles& particles)
    : theta(theta), particles(particles) {
    // build the quadtree
}

bool Quadtree::MAC(const glm::vec2& pos, const Node& n) const { return false; }

int Quadtree::NodeLevel(const Node& node) const { return 0; }

float Quadtree::NodeSize(const Node& node) const { return 0.0; }

Node& Quadtree::NodeParent(const Node& node) const {}

Node& Quadtree::NodeChild(const Node& node, int i) const {}

Node* Quadtree::Get(morton::MortonKey key) {
    auto result = hashtbl.find(key);
    if (result == hashtbl.end()) {
        return nullptr;
    } else {
        return &(result->second);
    }
}
void Quadtree::Set(morton::MortonKey key, const Node& node) {}

glm::vec2 Quadtree::CalcAccel(size_t i) const { return glm::vec2(); }
// const float G = 6.67430e-11f;

// QuadTree::QuadTree(const Grid& grid) : grid(grid) {
//     // Initialize root cell
//     Node& root = cells[1];  // Key 1 is root (placeholder bit set)
//     root.key = 1;
// }

// void QuadTree::Build(const std::vector<glm::vec2>& positions,
//                      const std::vector<float>& masses) {
//     Clear();  // Reset the tree

//     // Insert each particle
//     for (size_t i = 0; i < positions.size(); i++) {
//         // Convert position to grid coordinates and get Morton key
//         auto idx = grid.Snap(positions[i]);
//         morton::MortonKey key = morton::encode(idx.first, idx.second);

//         // Create/update cell
//         Node& cell = cells[key];
//         cell.key = key;

//         // Update mass properties
//         float new_total_mass = cell.total_mass + masses[i];
//         cell.center_of_mass =
//             (cell.center_of_mass * cell.total_mass + positions[i] * masses[i]) /
//             new_total_mass;
//         cell.total_mass = new_total_mass;
//         cell.particle_indices.push_back(i);

//         // Update parent cells
//         morton::MortonKey parent_key = key >> 2;  // Get parent by right shift
//         while (parent_key != 0) {
//             Node& parent = cells[parent_key];
//             int child_idx = key & 3;  // Get last 2 bits
//             parent.child_mask |= (1 << child_idx);

//             // Update parent's mass properties
//             parent.total_mass += masses[i];
//             parent.center_of_mass =
//                 (parent.center_of_mass * (parent.total_mass - masses[i]) +
//                  positions[i] * masses[i]) /
//                 parent.total_mass;

//             key = parent_key;
//             parent_key = key >> 2;
//         }
//     }
// }

// Node* QuadTree::Find(morton::MortonKey key) {
//     auto it = cells.find(key);
//     return it != cells.end() ? &it->second : nullptr;
// }

// const Node* QuadTree::Find(morton::MortonKey key) const {
//     auto it = cells.find(key);
//     return it != cells.end() ? &it->second : nullptr;
// }

// glm::vec2 QuadTree::CalcAccel(size_t i, const glm::vec2& pos, float mass,
//                               float theta) const {
//     // Start from root (key = 1) and recursively calculate forces
//     glm::vec2 total_force = CalcForceFromCell(Find(1), pos, theta * theta);
//     return total_force / mass;
// }

// glm::vec2 QuadTree::CalcForceFromCell(const Node* cell, const glm::vec2& pos,
//                                       float theta_squared) const {
//     if (!cell || cell->total_mass == 0) return glm::vec2(0.0f);

//     // Calculate displacement vector and distance squared
//     glm::vec2 diff = cell->center_of_mass - pos;
//     float dist_squared = diff.x * diff.x + diff.y * diff.y;

//     // Get cell size based on level in tree
//     float cell_size = grid.scale / (1 << (GetLevel(cell->key)));

//     // If cell is far enough away OR is a leaf, use approximation
//     if (cell->child_mask == 0 || cell_size * cell_size < dist_squared * theta_squared)
//     {
//         float dist = sqrt(dist_squared);
//         return G * cell->total_mass * diff / (dist_squared * dist);
//     }

//     // Otherwise, recursively compute forces from children
//     glm::vec2 total_force(0.0f);
//     morton::MortonKey first_child = cell->key << 2;
//     for (int i = 0; i < 4; i++) {
//         if (cell->child_mask & (1 << i)) {
//             total_force += CalcForceFromCell(Find(first_child + i), pos,
//             theta_squared);
//         }
//     }
//     return total_force;
// }

// int QuadTree::GetLevel(morton::MortonKey key) const {
//     int level = 0;
//     key = key >> 1;  // Skip placeholder bit
//     while (key > 0) {
//         key = key >> 2;
//         level++;
//     }
//     return level;
// }

}  // namespace nbody