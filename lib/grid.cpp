#include "grid.h"

namespace grid {

float Grid::Get(size_t i, size_t j) { return data[i * width + j]; }

float Grid::Get(std::pair<size_t, size_t> idx) {
    return Grid::Get(idx.first, idx.second);
}

void Grid::Set(size_t i, size_t j, float value) { data[i * width + j] = value; }

void Grid::Set(std::pair<size_t, size_t> idx, float value) {
    return Grid::Set(idx.first, idx.second, value);
}

std::pair<size_t, size_t> Grid::Snap(float x, float y) {
    return std::make_pair(static_cast<size_t>(y / CELL_HEIGHT - this->y),
                          static_cast<size_t>(x / CELL_WIDTH - this->x));
}

size_t next_pow_two(size_t i) {
    // see https://stackoverflow.com/a/77787296
    i = i > 2 ? i : 2;
    int32_t nlz = __builtin_clz(i - 1);
    return nlz ? (1u << (32 - nlz)) : 0xffffffff;
}

void Grid::Configure(float ax, float ay, float bx, float by) {
    size_t nrows_raw = (by - ay) / CELL_HEIGHT;
    size_t ncols_raw = (bx - ax) / CELL_WIDTH;
    // round up to smallest power of two
    nrows = next_pow_two(nrows_raw);
    ncols = next_pow_two(ncols_raw);
    // center the expanded grid around the bounding box
    x = ax - (ncols - ncols_raw) / 2.0f;
    y = ay - (nrows - nrows_raw) / 2.0f;

    data.clear();
    data.resize(nrows * ncols, 0.0f);
}

}  // namespace grid