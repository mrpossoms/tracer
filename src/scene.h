#pragma once

#include "base.h"

namespace Tracer {

enum IntRes {
	INT_FALSE = 0,
	INT_POSSIBLE,
	INT_TRUE = 1,

};

struct Intersection;
struct Surface;

struct MaterialSample {
	Vec3 reflectance;
	Vec3 emittance;
};

struct Material {
	virtual MaterialSample sample(Intersection* i) = 0;
};

struct Intersection {
	Surface* surf;
	float t;
	Ray3 incident;
	Vec3 point;
	Vec3 normal;
	MaterialSample sample;
};

struct Surface {
	Material* material;
	virtual IntRes intersectsAt(Ray3& ray, Intersection* i) = 0;
	virtual void   reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing) = 0;
	virtual void   transform(mat4x4 m) = 0;
};

struct Viewer {
	float near, far;
	mat4x4 view;

	Viewer(float near, float far, float fov, float aspect);

	// view matrix
	Viewer& position(Vec3 pos);
	Viewer& look_at(Vec3 targ);
	Viewer& up(Vec3 up);

	// perspective matrix
	Viewer& field_of_view(float fov);

	void recalculate();

private:
	Vec3 _pos, _up, _target;
	float fov, aspect;
	bool dirtyProj, dirtyView;
};

struct Scene {
	Viewer view;
	Surface** surfaces; // terminated with null

	bool intersected(Ray3& ray, Intersection* i);
};

}
