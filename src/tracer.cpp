#include "tracer.h"
#include <string.h>

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

	if(depth >= 1)
	{
		i->surf = opts.int_list[depth - 1].surf;
		// assert(opts.int_list[depth - 1].surf != opts.int_list[depth].surf);
	}

	// Test to see if we are finished, or if we've hit something
	if(depth >= opts.max_depth) return black;
	if(!opts.scene->intersected(ray, i)) return black;


	MaterialSample mat = i->sample;

	if(i->surf->material->no_reflection) return mat.emittance;

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
	static bool rng_tabs_generated;
	size_t pix_size = info.pixel_format * sizeof(uint8_t);
	size_t row_size = pix_size * info.width;
	const int MAX_DEPTH = 3;

	if(!rng_tabs_generated)
	{
		init_rng();
		rng_tabs_generated = true;
	}

	TraceOpts opts = {
		.scene     = scene,
		.max_depth = MAX_DEPTH
	};

	char* spectrum = " .,':;|[{+*X88";
	int spec_size = strlen(spectrum) - 1;

	// a character is roughly twice as tall as as it is
	// wide, so when scaling the x axis for the UVs the scaling
	// coefficent (aspect) must take that into account.
	float aspect = info.width / (float)(info.height * 2);

	for(int y = info.height; y--;)
	{
		uint8_t* row = info.buffer + row_size * y;
		for(int x = info.width; x--;)
		{
			uint8_t* pix = row + x * pix_size;
			float u = (((x << 1) + 1) / (float)info.width)  - 1.f;
			float v = (((y << 1) + 1) / (float)info.height) - 1.f;
			Ray3 ray = ray_at_coord(u * aspect, v, scene->view);
			Vec3 color(0, 0, 0);

			const float samples = 10;
			for(int i = samples; i--;)
			{
				Intersection int_list[MAX_DEPTH] = {};
				opts.int_list = int_list;
				color += trace_path(ray, opts, 0);
			}

			color *= (1.f / samples);

			color.x = color.x > 1 ? 1 : color.x;
			color.y = color.y > 1 ? 1 : color.y;
			color.z = color.z > 1 ? 1 : color.z;

			pix[0] = spectrum[(int)(color.x * spec_size)];
			pix[1] = spectrum[(int)(color.y * spec_size)];
			pix[2] = spectrum[(int)(color.z * spec_size)];
		}
	}

	return 0;
}
