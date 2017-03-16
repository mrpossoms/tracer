#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include "tracer.h"

using namespace Tracer;

int main(int argc, char* argv[])
{
	BufferInfo info = {
		.pixel_format = tr_fmt_rgb,
	};

	Light light;
	Plastic plastic;

	Sphere sphere0(Vec3(0, 0, 4), 1);
	Sphere sphere1(Vec3(0, 2, -2), 2);

	sphere0.material = &plastic;
	sphere1.material = &light;

	Surface* surfaces[] = { &sphere0, &sphere1, NULL };
	Scene scene = {
		.view = Viewer(1, 100, M_PI / 2, 1),
		.surfaces = surfaces,
	};

	initscr(); cbreak(); noecho();

	float t = 0;
	while(1)
	{
		getmaxyx(stdscr, info.height, info.width);
		uint8_t buf[info.width * info.height * 3];
		info.buffer = buf;

		sphere1.position.x = cos(t) * 6;
		sphere1.position.z = sin(t) * 6 + 4;
		t += 0.01;

		trace(&scene, info);

		for(int i = 0; i < info.height; ++i)
		{
			uint8_t* row = buf + (info.width * i * 3);
			move(i, 0);

			for(int j = info.width; j--;)
			{
				// write(1, row + (j * 3), 1);
				addch(row[j * 3]);
			}
		}



		refresh();
		usleep(1);
	}

	return 0;
}
