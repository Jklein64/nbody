#pragma once

#include <cstddef>
#include <cstdint>

#include "grid.h"

namespace morton {

using MortonKey = uint64_t;

MortonKey encode(nbody::GridIndex i, nbody::GridIndex j);
MortonKey encode(std::pair<nbody::GridIndex, nbody::GridIndex> idx);

std::pair<nbody::GridIndex, nbody::GridIndex> decode(MortonKey k);

int compare(MortonKey a, MortonKey b);

MortonKey parent(MortonKey key);

MortonKey first_child(MortonKey key);

MortonKey sibling(MortonKey first, int i);

}  // namespace morton