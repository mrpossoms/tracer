#include "geometries.h"

using namespace Tracer;

Plane::Plane(Vec3 position, Vec3 min, Vec3 max)
{
	this->position = position;
	setOrientation(QUAT_I);
	setCorners(min, max);

	tag = GEO_TAG_PLANE;
}

IntRes Plane::intersectsAt(Ray3& ray, Intersection* i)
{
	Vec3 r_d = ray.dir;
	float n_dot_rd = -vec3_mul_inner(normal.v, r_d.v);

	if(n_dot_rd <= 0)
	{
		return INT_FALSE;
	}

	Vec3 r_o = ray.pos - position;
	i->t     = -vec3_mul_inner(normal.v, r_o.v) / -n_dot_rd;
	i->point = r_d * i->t;

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

void Plane::setOrientation(Quat Q)
{
	this->q = Q;

	quat_mul_vec3(min.v, Q.v, min_o.v);
	quat_mul_vec3(max.v, Q.v, max_o.v);

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

void Plane::transform(mat4x4 m)
{

}

void Plane::setCorners(Vec3 min, Vec3 max)
{
	this->min_o = min;
	this->max_o = max;

	setOrientation(q);
}
