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
	if(depth >= opts.max_depth) return black;
	if(!opts.scene->intersected(ray, i)) return black;
	MaterialSample mat = i->sample;

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

Ray3 ray_at_coord(float u, float v, Viewer& view)
{
	Ray3 ray;
	ray.pos = Vec3(u, v, view.near);
	ray.dir = Vec3(-u, -v, view.near);

	vec3_norm(ray.dir.v, ray.dir.v);

	return ray;
}

int Tracer::trace(Scene* scene, BufferInfo info)
{
	size_t pix_size = info.pixel_format * sizeof(uint8_t);
	size_t row_size = pix_size * info.width;
	const int MAX_DEPTH = 10;

	Intersection int_list[MAX_DEPTH];
	TraceOpts opts = {
		.scene     = scene,
		.int_list  = int_list,
		.max_depth = MAX_DEPTH
	};

	for(int y = info.height; y--;)
	{
		uint8_t* row = info.buffer + row_size * y;
		for(int x = info.width; x--;)
		{
			uint8_t* pix = row + x * pix_size;
			float u = ((x << 1) + 1) / (float)info.width;
			float v = ((y << 1) + 1) / (float)info.height;
			Ray3 ray = ray_at_coord(u - 1, v - 1, scene->view);
			Vec3 color = trace_path(ray, opts, 0);

			pix[0] = color.x * 9 + 48;
			pix[1] = color.y * 9 + 48;
			pix[2] = color.z * 9 + 48;
		}
	}

	return 0;
}
