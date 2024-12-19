#include "grid.h"

namespace nbody {

Grid::Grid() : scale(0), height(0), width(0), x(0), y(0), nrows(0), ncols(0) {}

float Grid::Get(size_t i, size_t j) const { return data[i * width + j]; }

float Grid::Get(std::pair<size_t, size_t> idx) const {
    return Grid::Get(idx.first, idx.second);
}

void Grid::Set(size_t i, size_t j, float value) { data[i * width + j] = value; }

void Grid::Set(std::pair<size_t, size_t> idx, float value) {
    return Grid::Set(idx.first, idx.second, value);
}

std::pair<size_t, size_t> Grid::Snap(glm::vec2 pos) const {
    return std::make_pair(static_cast<size_t>((pos.y - this->y) / scale + 0.5f),
                          static_cast<size_t>((pos.x - this->x) / scale + 0.5f));
}

size_t next_pow_two(size_t i) {
    // see https://stackoverflow.com/a/77787296
    i = i > 2 ? i : 2;
    int32_t nlz = __builtin_clz(i - 1);
    return nlz ? (1u << (32 - nlz)) : 0xffffffff;
}

void Grid::Configure(float scale, glm::vec2 a, glm::vec2 b) {
    this->scale = scale;
    size_t nrows_raw = (b.y - a.y) / scale;
    size_t ncols_raw = (b.x - a.x) / scale;
    // round up to smallest power of two
    nrows = next_pow_two(nrows_raw);
    ncols = next_pow_two(ncols_raw);
    // center the expanded grid around the bounding box
    x = a.x - (ncols - ncols_raw) / 2.0f;
    y = a.y - (nrows - nrows_raw) / 2.0f;

    data.clear();
    data.resize(nrows * ncols, 0.0f);
}

}  // namespace nbody