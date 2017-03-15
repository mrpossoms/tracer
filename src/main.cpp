#include <math.h>
#include <unistd.h>
#include "tracer.h"

using namespace Tracer;

int main(int argc, char* argv[])
{
	const int w = 40, h = 15;

	uint8_t buf[w * h * 3] = {};

	BufferInfo info = {
		.pixel_format = tr_fmt_rgb,
		.width = w,
		.height = h,
		.buffer = buf,
	};

	Sphere sphere(Vec3(0, 0, 4), 1);
	Surface* surfaces[] = { &sphere, NULL };
	Scene scene = {
		.view = Viewer(1, 100, M_PI / 2, 1),
		.surfaces = surfaces,
	};

	trace(&scene, info);

	for(int i = info.height; i--;)
	{
		uint8_t* row = buf + (info.width * i * 3);

		for(int j = info.width; j--;)
		{
			write(1, row + (j * 3), 1);
		}
		write(1, "\n", 1);
	}

	return 0;
}
