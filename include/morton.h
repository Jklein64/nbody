#pragma once

#include <cstddef>

namespace morton {

typedef size_t MortonKey;

MortonKey encode(size_t i, size_t j);

void decode(MortonKey k, size_t* i, size_t* j);

int compare(MortonKey a, MortonKey b);

}  // namespace morton