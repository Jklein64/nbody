#include "grid.h"

namespace grid {

Grid::Grid(size_t width_, size_t height_, float cell_width_, float cell_height_)
    : width(width_), height(height_), cell_width(cell_width_), cell_height(cell_height_) {
    data_.reserve(width * height);
    data_.resize(width * height, 0.0f);
}

float Grid::get(size_t i, size_t j) { return data_[i * width + j]; }

float Grid::get(std::pair<size_t, size_t> idx) {
    return Grid::get(idx.first, idx.second);
}

void Grid::set(size_t i, size_t j, float value) { data_[i * width + j] = value; }

void Grid::set(std::pair<size_t, size_t> idx, float value) {
    return Grid::set(idx.first, idx.second, value);
}

std::pair<size_t, size_t> Grid::snap(float x, float y) {
    return std::make_pair(static_cast<size_t>(y / cell_height),
                          static_cast<size_t>(x / cell_width));
}

const std::vector<float>& Grid::ViewFlattened() { return data_; }

}  // namespace grid