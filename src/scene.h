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
	bool no_reflection;
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
	int tag;
	virtual IntRes intersectsAt(Ray3& ray, Intersection* i) = 0;
	virtual void   reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing) = 0;
	virtual void   transform(mat4x4 m) = 0;
};

struct Viewer {
	float near, far;

	Viewer(float near, float far, float fov, float aspect);

	// view matrix
	Viewer& position(Vec3 pos);
	Viewer& look_at(Vec3 targ);
	Viewer& up(Vec3 up);
	Viewer& view(mat4x4 v);

	// perspective matrix
	Viewer& field_of_view(float fov);

	Ray3 ray_at_coord(float u, float v);

	void recalculate();

private:
	mat4x4 _view;
	vec4 _pos, _up, _left, _target;
	float fov, aspect;
	bool dirtyProj, dirtyView;
};

struct Scene {
	Viewer view;
	Surface** surfaces; // terminated with null

	bool intersected(Ray3& ray, Intersection* i);
};

}
