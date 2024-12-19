#pragma once

#include <cstddef>
#include <cstdint>

#include "grid.h"

namespace morton {

using MortonKey = uint64_t;

MortonKey encode(uint32_t i, uint32_t j);

void decode(MortonKey k, nbody::GridIndex* i, nbody::GridIndex* j);

int compare(MortonKey a, MortonKey b);

MortonKey parent(MortonKey key);

MortonKey first_child(MortonKey key);

MortonKey sibling(MortonKey first, int i);

}  // namespace morton