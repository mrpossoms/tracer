#include "materials.h"

using namespace Tracer;

MaterialSample Light::sample(Intersection* i)
{
	MaterialSample s = {
		.emittance = Vec3(2, 2, 2),
		.reflectance = Vec3(0, 0, 0),
	};

	return s;
}

MaterialSample Plastic::sample(Intersection* i)
{
	MaterialSample s = {
		.emittance = Vec3(0.05, 0.05, 0.05),
		.reflectance = Vec3(0.5, 0.5, 0.5),
	};

	return s;
}
