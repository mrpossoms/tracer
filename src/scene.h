#pragma once

#include "base.h"

namespace Tracer {

enum IntRes {
	INT_FALSE = 0,
	INT_POSSIBLE,
	INT_TRUE,

};

struct Material {
	Vec3 reflectance;
	Vec3 emittance;
};

struct Surface;
struct Intersection {
	Surface* surf;
	Vec3 point;
	Vec3 normal;
	Material material;
};

struct Surface {
	IntRes intersectsAt(Ray3& ray, Intersection* i);
	void   reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing);
};

struct Viewer {
	mat4x4 projection, view;
};

struct Scene {
	Viewer view;
	Surface** surfaces; // terminated with null

	bool intersected(Ray3& ray, Intersection* i);
};

}
