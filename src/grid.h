#pragma once

#include <stddef.h>

#include <glm/vec2.hpp>
#include <vector>

namespace nbody {

using GridIndex = uint32_t;

struct Grid {
    float scale;                // width and height of a cell
    size_t nrows, ncols;        // grid dimensions
    float x, y, width, height;  // bounding box of the grid

    // data[i * ncols + j] is the mass at cell i, j
    std::vector<float> data;
    // indices[i * ncols + j] is a list of indices of particles contained in cell i, j
    std::vector<std::vector<size_t>> indices;

    Grid();

    float Get(GridIndex i, GridIndex j) const;
    float Get(std::pair<GridIndex, GridIndex> idx) const;
    void Set(GridIndex i, GridIndex j, float value);
    void Set(std::pair<GridIndex, GridIndex> idx, float value);
    std::pair<GridIndex, GridIndex> Snap(glm::vec2 pos) const;

    // (Re)configures this grid to fit the bbox with a in top left and b in bottom right
    void Configure(float scale, glm::vec2 a, glm::vec2 b);
};

}  // namespace nbody
