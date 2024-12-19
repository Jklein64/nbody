#include "morton.h"

#include "grid.h"

namespace morton {

template <typename T>
bool bit_get(const T& v, int i) {
    return (v >> i) & 1;
}

template <typename T>
void bit_set(T& v, size_t i, bool b) {
    // assumes all modifiable bits in v are zero
    v |= (int)b << i;
}

MortonKey encode(nbody::GridIndex i, nbody::GridIndex j) {
    // placeholder MSB, rest zero
    MortonKey key = 1ull << 63;
    // interleave the rest of the bits
    for (int k = 0; k < 31; k++) {
        // ignore MSB of each index so that we can actually
        // fit the index into the morton key (31 + 31 + 1 = 63 < 64)
        bit_set(key, 2 * k, bit_get(i, k));
        bit_set(key, 2 * k + 1, bit_get(j, k));
    }

    return key;
}

MortonKey encode(std::pair<nbody::GridIndex, nbody::GridIndex> idx) {
    return encode(idx.first, idx.second);
}

std::pair<nbody::GridIndex, nbody::GridIndex> decode(MortonKey key) {
    // zero out for bit_set
    nbody::GridIndex i = 0, j = 0;
    for (int k = 0; k < 31; k++) {
        bit_set(i, k, bit_get(key, 2 * k));
        bit_set(j, k, bit_get(key, 2 * k + 1));
    }
    return std::make_pair(i, j);
}

int compare(MortonKey a, MortonKey b) {
    if (a < b)
        return -1;
    else if (a > b)
        return 1;
    return 0;
}

// Get parent key by right shifting 2 bits (removes last x,y coordinate)
MortonKey parent(MortonKey key) { return key >> 2; }

// Get first child key by left shifting 2 bits
MortonKey first_child(MortonKey key) { return key << 2; }

// Get sibling keys by adding 1-3 to first child
MortonKey sibling(MortonKey first, int i) { return first + i; }

}  // namespace morton