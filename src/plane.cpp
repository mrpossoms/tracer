#include "geometries.h"

using namespace Tracer;

Plane::Plane(Vec3 position, Vec3 min, Vec3 max)
{
	this->position = position;
	setCorners(min, max);

	tag = GEO_TAG_PLANE;
}

IntRes Plane::intersectsAt(Ray3& ray, Intersection* i)
{
	// TODO transform ray
	Vec3 r_d = ray.dir;

	float n_dot_rd = -vec3_mul_inner(normal.v, r_d.v);

	if(n_dot_rd <= 0)
	{
		return INT_FALSE;
	}

	// TODO transform ray
	Vec3 r_o = ray.pos - position;
	i->t     = (vec3_mul_inner(normal.v, position.v) - vec3_mul_inner(normal.v, ray.pos.v)) / -n_dot_rd;

	// if(i->t <= 0)
	// {
	// 	return INT_FALSE;
	// }

	i->point = ray.dir * i->t;

	// ensure that the point is contained within
	// the plane's bounds
	Vec3 tp = i->point + r_o;
	Vec3 w = tp - max;
	w /= diag;

	// // I hate this...
	if(w.x < 0 || w.x > 1 || w.y < 0 || w.y > 1 || w.z < 0 || w.z > 1)
	{
		return INT_FALSE;
	}

	// i->normal = normal;
	float s = (i->t < 0 ? -1 : 1);
	i->point += ray.pos;
	i->normal = normal * s;
	i->t *= s;

	i->surf   = this;
	i->sample = material->sample(i);

	return INT_TRUE;
}

void Plane::reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing)
{

	outgoing.pos = i.point;
	outgoing.dir = i.normal * 2;

	Vec3 rnd_cube(RNG_NEXT, RNG_NEXT, RNG_NEXT);
	outgoing.dir += rnd_cube;

	vec3_norm(outgoing.dir.v, outgoing.dir.v);

}

void Plane::transform(mat4x4 m)
{

}

void Plane::setCorners(Vec3 min, Vec3 max)
{
	this->min = min;
	this->max = max;
	Vec3 corner(min.x, max.y, min.z);
	Vec3 edges[2] = {
		min - corner,
		corner - max,
	};

	vec3_mul_cross(this->normal.v, edges[0].v, edges[1].v);
	vec3_norm(this->normal.v, this->normal.v);
	this->diag = min - max;

	// bias a bit
	for(int i = 3; i--;)
	{
		if(diag.v[i] == 0) diag.v[i] += 1e-6;
	}
}
