#include "geometries.h"
#include <assert.h>

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
	// If this was the last surface intersected, don't
	// allow self reflection. It's not possible with a plane
	if(i->surf == this) return INT_FALSE;

	Vec3 r_d;
	quat_mul_vec3(r_d.v, inv_q.v, ray.dir.v);

	float ns_dot_rd[] = {
		-normal[0].dot(r_d),
		-normal[1].dot(r_d),
	};

	int n_i = ns_dot_rd[0] > 0 ? 0 : 1;
	float n_dot_rd = ns_dot_rd[n_i];

	if(n_dot_rd == 0) return INT_FALSE;

	Vec3 r_o = ray.pos - position;
	quat_mul_vec3(r_o.v, inv_q.v, r_o.v);
	i->t     = vec3_mul_inner(normal[n_i].v, r_o.v) / n_dot_rd;
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

	// float s = (i->t < 0 ? -1 : 1);
	i->point += ray.pos;
	// i->normal = normal[n_i];
	quat_mul_vec3(i->normal.v, q.v, normal[n_i].v);

	// i->t *= s;
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
	this->inv_q = Q;
	this->inv_q.invert();

	// min = min_o;
	// max = max_o;
	// Vec3 corner = corner_o;
	//
	// quat_mul_vec3(min.v, q.v, min_o.v);
	// quat_mul_vec3(max.v, q.v, max_o.v);
	// quat_mul_vec3(corner.v, q.v, corner_o.v);
	//
	// Vec3 edges[2] = {
	// 	min - corner,
	// 	corner - max,
	// };
	//
	// vec3_mul_cross(this->normal[0].v, edges[0].v, edges[1].v);
	// vec3_norm(this->normal[0].v, this->normal[0].v);
	// this->diag = min - max;
	//
	// // bias a bit
	// for(int i = 3; i--;)
	// {
	// 	if(diag.v[i] == 0) diag.v[i] += 1e-6;
	// }
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

	vec3_mul_cross(this->normal[0].v, edges[0].v, edges[1].v);
	vec3_norm(this->normal[0].v, this->normal[0].v);
	this->normal[1] = this->normal[0] * -1.f;
	this->diag = min - max;

	// bias a bit
	for(int i = 3; i--;)
	{
		if(diag.v[i] == 0) diag.v[i] += 1e-6;
	}
	// setOrientation(q);
}
