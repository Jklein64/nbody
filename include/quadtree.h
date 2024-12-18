#pragma once

#include <vector>
#include <unordered_map>
#include <glm/vec2.hpp>

#include "morton.h"
#include "../src/grid.h"

namespace nbody {

struct Cell {
    glm::vec2 center_of_mass;         // Center of mass of all particles in cell
    float total_mass;                 // Total mass of particles in cell
    uint8_t child_mask;               // Bits indicating which children exist
    morton::MortonKey key;            // Spatial key for hashing
    std::vector<size_t> particle_indices;  // Indices of particles in this cell
    
    Cell() : center_of_mass(0.0f), total_mass(0.0f), child_mask(0), key(0) {}
};

class QuadTree {
public:
    QuadTree(const Grid& grid);
    
    // Build tree from particles
    void Build(const std::vector<glm::vec2>& positions,
              const std::vector<float>& masses);
              
    // Get acceleration on a particle
    glm::vec2 CalcAccel(size_t i, const glm::vec2& pos, float mass, float theta) const;
    
    // Clear the tree
    void Clear() { cells.clear(); }

private:
    std::unordered_map<morton::MortonKey, Cell> cells;
    const Grid& grid;
    
    // Helper functions
    Cell* Find(morton::MortonKey key);
    const Cell* Find(morton::MortonKey key) const;
    int GetLevel(morton::MortonKey key) const;
    glm::vec2 CalcForceFromCell(const Cell* cell, const glm::vec2& pos, 
                               float theta_squared) const;
};

} // namespace nbody