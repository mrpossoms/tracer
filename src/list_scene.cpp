#include "scene.h"

using namespace Tracer;

bool Scene::intersected(Ray3& ray, Intersection* intr)
{
	bool intersected = false;

	for(int j = 0; surfaces[j]; ++j)
	{
		Intersection temp = {};
		bool last = intersected;

		intersected = surfaces[j]->intersectsAt(ray, &temp) | intersected;

		if(!intersected) continue;
		if(last != intersected) *intr = temp;

		if(temp.t < intr->t)
		{
			*intr = temp;
		}
	}

	return intersected;
}
