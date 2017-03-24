#include "scene.h"
#include "geometries.h"
#include <unistd.h>

using namespace Tracer;

bool Scene::intersected(Ray3& ray, Intersection* intr)
{
	Surface* last_surf = intr->surf;
	bool ever_intersected = false;

	for(int j = 0; surfaces[j]; ++j)
	{
		Intersection surf_int = {
			.surf = last_surf,
		};
		bool last = ever_intersected;
		bool intersected = surfaces[j]->intersectsAt(ray, &surf_int);

		ever_intersected |= intersected;

		if(!intersected) continue;
		if(last != ever_intersected) *intr = surf_int;

		if(surf_int.t < intr->t)
		{
			*intr = surf_int;
		}
	}

	return ever_intersected;
}
