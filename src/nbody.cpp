#include "nbody.h"

namespace nbody {

void init(const SimParams& params, Particle (*sampler)()) {}
void step(const SimParams& params, const SimState& state) {}
void free(const SimState& state) {}

}  // namespace nbody