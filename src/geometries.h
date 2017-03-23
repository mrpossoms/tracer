#pragma once

#include "scene.h"

#define GEO_TAG_SPHERE 0
#define GEO_TAG_PLANE  1

namespace Tracer {

struct Sphere : Surface {
	Vec3  position;
	float radius;

	Sphere(Vec3 position, float radius);
	IntRes intersectsAt(Ray3 ray, Intersection* i);
	void reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing);
	void transform(mat4x4 m);

private:
	Vec3 trans_position;
};

struct Box : Surface {
	Vec3 min, max;
	Vec3 position;
	Quat orientation;

	Box(Vec3 position, Vec3 min, Vec3 max);
	IntRes intersectsAt(Ray3& ray, Intersection* i);
	void reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing);
	void transform(mat4x4 m);

private:
	Vec3 po[6]; // plane origins
};

struct Plane : Surface {
	Vec3 position;


	Plane(Vec3 position, Vec3 min, Vec3 max);
	IntRes intersectsAt(Ray3 ray, Intersection* i);
	void reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing);
	void transform(mat4x4 m);

	void setCorners(Vec3 min, Vec3 max);
	void setOrientation(Quat q);

private:
	Vec3 min_o, max_o, corner_o;
	Vec3 normal, min, max, diag;
	Quat q, inv_q;
};

}
