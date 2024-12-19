#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <vector>

#include "nbody.h"

namespace nbody {

// spans a power-of-two-squared region of grid cells;
// leaf nodes are single grid cells
struct Node {
    glm::vec2 com;              // center of mass of contained bodies
    int com_n;                  // count for cumulative average
    glm::vec2 center;           // geometric center, for bounds testing
    float size;                 // length of a side
    float mass;                 // total mass of contained bodies
    std::array<int, 4> bodies;  // bodies[i] for quadrant i
    std::array<std::shared_ptr<Node>, 4> quadrants;
};

class Quadtree {
    float theta;
    std::shared_ptr<Node> root;
    const nbody::Particles& particles;

    // evaluates the Multipole Acceptance Criterion
    bool MAC(const glm::vec2& pos, const Node& n) const;

    // calculates the number of layers from the root (i.e. root has level 0)
    int NodeLevel(const Node& node) const;

    // calculates the side length of the given node in meters
    float NodeSize(const Node& node) const;

    // retrieves the parent of the node
    Node& NodeParent(const Node& node) const;

    // retrieves the i'th child of the node for i = 0...3
    Node& NodeChild(const Node& node, int i) const;

    int CalcQuadrant(const Node& node, const glm::vec2& pos) const;

   public:
    Quadtree(float theta, const nbody::Particles& particles);
    glm::vec2 CalcAccel(size_t i) const;
};

// // grid is made of cells
// // quadtree is made of nodes
// struct Node {
//     glm::vec2 center_of_mass;              // Center of mass of all particles in cell
//     float total_mass;                      // Total mass of particles in cell
//     uint8_t child_mask;                    // Bits indicating which children exist
//     morton::MortonKey key;                 // Spatial key for hashing
//     std::vector<size_t> particle_indices;  // Indices of particles in this cell

//     Node() : center_of_mass(0.0f), total_mass(0.0f), child_mask(0), key(0) {}
// };

// class QuadTree {
//    public:
//     QuadTree(const Grid& grid);

//     // Build tree from particles
//     void Build(const std::vector<glm::vec2>& positions, const std::vector<float>&
//     masses);

//     // Get acceleration on a particle
//     glm::vec2 CalcAccel(size_t i, const glm::vec2& pos, float mass, float theta) const;

//     // Clear the tree
//     void Clear() { cells.clear(); }

//    private:
//     std::unordered_map<morton::MortonKey, Node> cells;
//     const Grid& grid;

//     // Helper functions
//     Node* Find(morton::MortonKey key);
//     const Node* Find(morton::MortonKey key) const;
//     int GetLevel(morton::MortonKey key) const;
//     glm::vec2 CalcForceFromCell(const Node* cell, const glm::vec2& pos,
//                                 float theta_squared) const;
// };

}  // namespace nbody