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

    float Get(size_t i, size_t j);
    float Get(std::pair<size_t, size_t> idx);
    void Set(size_t i, size_t j, float value);
    void Set(std::pair<size_t, size_t> idx, float value);
    void Clear();
    std::pair<size_t, size_t> Snap(float x, float y);
    const std::vector<float>& ViewFlattened();

   private:
    // stored row-major internally
    std::vector<float> data_;
};

}  // namespace grid
