#pragma once

#include "linmath.h"

#define RNG_TAB_SIZE 1024

#define RNG_NEXT (RNG_TAB[(RNG_IDX = (++RNG_IDX % RNG_TAB_SIZE))])

extern float RNG_TAB[RNG_TAB_SIZE];
extern int RNG_IDX;

namespace Tracer {

struct Ray3 {
	Vec3 pos, dir;
};

void init_rng();

};
