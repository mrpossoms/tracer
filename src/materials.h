#pragma once

#include "scene.h"

namespace Tracer {

struct Light : Material {
	MaterialSample sample(Intersection* i);
};

struct Plastic : Material {
	MaterialSample sample(Intersection* i);
};

}
