#include "tracer.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define RENDERING_THREADS 4

static pthread_t THREADS[RENDERING_THREADS];

using namespace Tracer;

struct TraceOpts {
	Scene* scene;
	Intersection* int_list;
	int max_depth;
};

struct ThreadParams {
	Scene* scene;
	BufferInfo info;
	unsigned int start_x, end_x;
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

	const Vec3 scl(-1, -1, 1);
	// u = u / tanf(u * M_PI / 4);
	// v = v / tanf(v * M_PI / 4);

	Vec3 near(u, v, view.near);
	// Vec3 far(u * 2, v * 2, view.far);

	ray.pos = Vec3(u, v, view.near);
	ray.dir = ray.pos;
	ray.dir *= scl;

	vec3_norm(ray.pos.v, ray.pos.v);
	vec3_norm(ray.dir.v, ray.dir.v);

	return ray;
}

//char* SPECTRUM = " .,':;|[{+*X88";
char* SPECTRUM = (char*)"8X*+{[|;:',.  ";


static void* sub_trace(void* ctx)
{
	ThreadParams params = *(ThreadParams*)ctx;
	BufferInfo info = params.info;
	static bool rng_tabs_generated;
	size_t pix_size = info.pixel_format * sizeof(uint8_t);
	size_t row_size = pix_size * info.width;
	const int MAX_DEPTH = 3;

	if(!rng_tabs_generated)
	{
		init_rng();
		rng_tabs_generated = true;
	}

	TraceOpts opts = {};
	opts.scene     = params.scene;
	opts.max_depth = MAX_DEPTH;

	// a character is roughly twice as tall as as it is
	// wide, so when scaling the x axis for the UVs the scaling
	// coefficent (aspect) must take that into account.
	float aspect = info.width / (float)(info.height * 2);
	int sub_section_width = params.end_x - params.start_x;

	for(int y = info.height; y--;)
	{
		uint8_t* row = info.buffer + row_size * y;
		int spec_size = strlen(SPECTRUM) - 1;
		for(int x = params.start_x; x < params.end_x; ++x)
		{
			uint8_t* pix = row + x * pix_size;
			float u = (((x << 1) + 1) / (float)info.width)  - 1.f;
			float v = (((y << 1) + 1) / (float)info.height) - 1.f;
			Ray3 ray = ray_at_coord(u * aspect, v, params.scene->view);
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

			pix[0] = SPECTRUM[(int)(color.x * spec_size)];
			pix[1] = SPECTRUM[(int)(color.y * spec_size)];
			pix[2] = SPECTRUM[(int)(color.z * spec_size)];
		}
	}

	return NULL;
}


int Tracer::trace(Scene* scene, BufferInfo info)
{
	ThreadParams params[RENDERING_THREADS];
	pthread_t threads[RENDERING_THREADS];

	unsigned int per_thread = info.width / RENDERING_THREADS;

	for(int i = RENDERING_THREADS; i--;)
	{
		params[i].scene = scene;
		params[i].info  = info;
		params[i].start_x = per_thread * i;
		params[i].end_x   = (per_thread * i) + per_thread;

		if(i == RENDERING_THREADS - 1)
		{
			params[i].end_x = info.width;
		}

		if(info.debug)
		{
			printf("Thread%d [%d : %d]\n",
			       i,
			       params[i].start_x,
			       params[i].end_x
			);  
		}

		pthread_create(threads + i, NULL, sub_trace, params + i);
	}

	for(int i = RENDERING_THREADS; i--;) pthread_join(threads[i], NULL);

	return 0;
}
