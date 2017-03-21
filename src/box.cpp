#include "geometries.h"

Box::Box(Vec3 position, Vec3 min, Vec3 max)
{
	this->min = min;
	this->max = max;

	Vec3 half = min + max;
	half *= 0.5f;

	po[0] = Vec3(max.x,  half.y, half.z);
	po[1] = Vec3(half.x, max.y,  half.z);
	po[2] = Vec3(half.x, half.y, max.z);
	po[3] = Vec3(min.x,  half.y, half.z);
	po[4] = Vec3(half.x, min.y,  half.z);
	po[5] = Vec3(half.x, half.y, min.z);
}

IntRes Box::intersectsAt(Ray3& ray, Intersection* i)
{
	const Vec3 n[6] = {
		Vec3(1, 0, 0),  // n0
		Vec3(0, 1, 0),  // n1
		Vec3(0, 0, 1),  // n2
		Vec3(-1, 0, 0), // n3
		Vec3(0, -1, 0), // n4
		Vec3(0, 0, -1), // n5
	};

	const Vec3* adj_map[][4] = {
		{ n + 1, n + 2, n + 4, n + 5 }, // n0
		{ n + 0, n + 2, n + 3, n + 5 }, // n1
		{ n + 0, n + 1, n + 3, n + 4 }, // n2
		{ n + 1, n + 2, n + 4, n + 5 }, // n3
		{ n + 0, n + 2, n + 3, n + 5 }, // n4
		{ n + 0, n + 1, n + 3, n + 4 }, // n5
	}


}

void Box::reflectAt(Intersection& i, Ray3& ray, Ray3& outgoing)
{

}

void Box::transform(mat4x4 m)
{

}
