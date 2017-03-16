#include "materials.h"

using namespace Tracer;

MaterialSample Light::sample(Intersection* i)
{
	MaterialSample s = {
		.emittance = Vec3(1, 1, 1),
		.reflectance = Vec3(0, 0, 0),
	};

	return s;
}

MaterialSample Plastic::sample(Intersection* i)
{
	MaterialSample s = {
		.emittance = Vec3(0.0, 0.0, 0.0),
		.reflectance = Vec3(0.5, 0.5, 0.5),
	};

	return s;
}
