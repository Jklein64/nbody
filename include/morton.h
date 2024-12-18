#pragma once

#include <cstddef>
#include <cstdint>

namespace morton {

typedef uint64_t MortonKey;

MortonKey encode(uint32_t i, uint32_t j);

void decode(MortonKey k, size_t* i, size_t* j);

int compare(MortonKey a, MortonKey b);

MortonKey parent(MortonKey key);

MortonKey firstChild(MortonKey key);

MortonKey sibling(MortonKey first, int i);

}  // namespace morton