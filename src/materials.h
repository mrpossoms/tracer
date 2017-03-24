#pragma once

#include "scene.h"

namespace Tracer {

struct Light : Material {
	Light();
	Light(Vec3 power);

	MaterialSample sample(Intersection* i);

	Vec3 power;
};

struct Plastic : Material {
	MaterialSample sample(Intersection* i);
};

struct BlackBody : Material {
	MaterialSample sample(Intersection* i);
};


}
