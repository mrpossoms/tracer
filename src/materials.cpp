#include "materials.h"

using namespace Tracer;

Light::Light()
{
	power = Vec3(2, 2, 2);
	no_reflection = true;
}

Light::Light(Vec3 power)
{
	this->power = power;
	no_reflection = true;
}

MaterialSample Light::sample(Intersection* i)
{
	MaterialSample s;
	s.emittance = power;
	s.reflectance = Vec3(0, 0, 0);

	return s;
}

MaterialSample Plastic::sample(Intersection* i)
{
	MaterialSample s;
	s.emittance = Vec3(0.05, 0.05, 0.05);
	s.reflectance = Vec3(0.5, 0.5, 0.5);

	return s;
}

MaterialSample BlackBody::sample(Intersection* i)
{
	MaterialSample s;
	s.emittance = Vec3(0.0, 0.0, 0.0);
	s.reflectance = Vec3(0.0, 0.0, 0.0);

	return s;
}
