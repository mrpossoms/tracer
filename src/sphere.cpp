#include "geometries.h"

using namespace Tracer;

Sphere::Sphere(Vec3 position, float radius)
{
	this->position = position;
	this->radius   = radius;
}

IntRes Sphere::intersectsAt(Ray3& ray, Intersection* i)
{
	Vec3  l   = position - ray.pos;
	float s   = vec3_mul_inner(l.v, ray.dir.v);
	float l_2 = vec3_mul_inner(l.v, l.v);
	float r_2 = radius * radius;
	float m_2, q, t;

	if (s < 0 && l_2 > r_2)
	{
		return INT_FALSE;
	}

	m_2 = l_2 - s * s;

	if (m_2 > r_2)
	{
		return INT_FALSE;
	}

	q = sqrtf(r_2 - m_2);

	if (l_2 > r_2)
	{
		t = s - q;
	}
	else
	{
		t = s + q;
	}

	i->point = ray.pos + ray.dir * t;
	i->normal = i->point - position;
	i->sample.emittance = Vec3(0.5, 0.5, 0.5);
	i->sample.reflectance = Vec3(0.1, 0.1, 0.1);
	i->surf = this;

	vec3_norm(i->normal.v, i->normal.v);

	return INT_TRUE;
}

void Sphere::reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing)
{
	vec3_reflect(outgoing.dir.v, ray.dir.v, i.normal.v);
}

void Sphere::transform(mat4x4 m)
{

}
