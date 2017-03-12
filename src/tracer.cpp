#include "tracer.h"

using namespace Tracer;

struct TraceOpts {
	Scene* scene;
	Intersection* int_list;
	int max_depth;
};

static Vec3 trace_path(Ray3& ray, TraceOpts& opts, int depth)
{
	static const Vec3 black(0, 0, 0);
	Intersection* i = opts.int_list + depth;

	// Test to see if we are finished, or if we've hit something
	if(depth > opts.max_depth) return black;
	if(!opts.scene->intersected(ray, i)) return black;
	Material& mat = i->material;

	// Reflect / refract the ray
	Ray3 newRay;
	i->surf->reflectAt(*i, ray, newRay);

	// Compute BRDF
	float cos_theta = vec3_mul_inner(newRay.dir.v, i->normal.v);
	Vec3 BRDF = mat.reflectance * (2.0f * cos_theta);
	Vec3 reflection = trace_path(newRay, opts, depth + 1);

	// Recurse
	return mat.emittance + (BRDF * reflection);
}

int Tracer::trace(Scene* scene, BufferInfo info)
{
	return 0;	
}
