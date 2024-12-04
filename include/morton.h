#pragma once

#include <cstddef>

typedef size_t morton_key_t;

morton_key_t morton_encode(size_t i, size_t j);

void morton_decode(morton_key_t k, size_t* i, size_t* j);

int morton_compare(morton_key_t a, morton_key_t b);