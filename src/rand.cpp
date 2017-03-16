#include "base.h"
#include <stdlib.h>
#include <time.h>

using namespace Tracer;

float RNG_TAB[RNG_TAB_SIZE];
int RNG_IDX;

void Tracer::init_rng()
{
	srandom(time(NULL));

	for(int i = RNG_TAB_SIZE; i--;)
	{
		RNG_TAB[i] = ((random() % 2048) / 1024.f) - 1.f;
	}

	RNG_IDX = 0;
}
