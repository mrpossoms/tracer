#pragma once

#include "scene.h"

namespace Tracer {

struct Sphere : Surface {
	Vec3  position;
	float radius;

	Sphere(Vec3 position, float radius);
	IntRes intersectsAt(Ray3& ray, Intersection* i);
	void reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing);
	void transform(mat4x4 m);

private:
	Vec3 trans_position;
};

}
