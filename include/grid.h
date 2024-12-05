#pragma once

#include <stddef.h>

#include <vector>

namespace grid {

struct Grid {
    float scale;  // width and height of a cell
    size_t nrows, ncols;
    float x, y, width, height;
    std::vector<float> data;

    float Get(size_t i, size_t j);
    float Get(std::pair<size_t, size_t> idx);
    void Set(size_t i, size_t j, float value);
    void Set(std::pair<size_t, size_t> idx, float value);
    std::pair<size_t, size_t> Snap(float x, float y);

    /**
     * @brief (Re)configures this grid to fit the bounding box defined with top left
     * corner ax and ay and bottom right corner bx and by.
     *
     * @param ax
     * @param ay
     * @param bx
     * @param by
     */
    void Configure(float scale, float ax, float ay, float bx, float by);
};

}  // namespace grid
