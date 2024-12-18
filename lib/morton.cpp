#include "morton.h"
#include <cstdint>

namespace morton {

// using MortonKey = uint64_t; // size_t;

MortonKey encode(uint32_t i, uint32_t j) {
    MortonKey key = 0;

    key = 1ull << 63; // (sizeof(MortonKey) * 8 - 1); // placeholder bit

    const int num_bits = 31; // (sizeof(MortonKey) * 8 - 1) / 2;

    for (int k = 0; k < num_bits; k++) {
        k |= ((i & (1ull << k)) << k);
        k |= ((j & (1ull << k)) << (k + 1));
    }

    return key;
}

void decode(MortonKey k, uint32_t* i, uint32_t* j) {
    *i = 0;
    *j = 0;

    const int num_bits = 31; //(sizeof(MortonKey) * 8 - 1)/ 2;

    for (int k = 0; k < num_bits; k++) {
        *i |= ((k & (1ull << (2*k))) >> k);
        *j |= ((k & (1ull << (2*k + 1))) >> (k+1));
    }
}

int compare(MortonKey a, MortonKey b) {
    if (a < b) return -1;
    else if (a > b) return 1;
    return 0;
}

// Get parent key by right shifting 2 bits (removes last x,y coordinate)
MortonKey parent(MortonKey key) {
    return key >> 2;
}

// Get first child key by left shifting 2 bits
MortonKey firstChild(MortonKey key) {
    return key << 2;
}

// Get sibling keys by adding 1-3 to first child
MortonKey sibling(MortonKey first, int i) {
    return first + i;
}

}  // namespace morton