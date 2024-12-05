#pragma once

#include <stddef.h>

#include <vector>

namespace grid {

class Grid {
   public:
    const size_t width;
    const size_t height;
    const float cell_width;
    const float cell_height;

    Grid(size_t width, size_t height, float cell_width, float cell_height);

    float get(size_t i, size_t j);
    float get(std::pair<size_t, size_t> idx);
    void set(size_t i, size_t j, float value);
    void set(std::pair<size_t, size_t> idx, float value);
    std::pair<size_t, size_t> snap(float x, float y);
    const std::vector<float>& ViewFlattened();

   private:
    // stored row-major internally
    std::vector<float> data_;
};

}  // namespace grid
