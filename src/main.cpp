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
	BlackBody blackbody;

	// Sphere subject(Vec3(0, 0, 4), 1);
	Plane subject(Vec3(0, 0, 4), Vec3(-0.75f, -0.75f, 0), Vec3(0.75f, 0.75f, 0));
	Sphere sphere0(Vec3(4, 0, 4), 1);
	Sphere sphere1(Vec3(0, 2, -2), 2);

	subject.material = &plastic;
	sphere0.material = &plastic;
	sphere1.material = &light;

	Surface* surfaces[] = { &subject, &sphere1, NULL, &sphere0,};
	Scene scene = {
		.view = Viewer(1, 100, M_PI / 2, 1),
		.surfaces = surfaces,
	};

	initscr(); cbreak(); noecho();

	float t = 0;
	Quat w, q;
	w.from_axis_angle(0, 1, 0, 0.005);

	while(1)
	{
		getmaxyx(stdscr, info.height, info.width);
		uint8_t buf[info.width * info.height * 3];
		info.buffer = buf;

		sphere1.position.x = cos(t) * 7;
		sphere1.position.y = 0;
		sphere1.position.z = sin(t) * 7;
		t += 0.01;
		q = q * w;
		// quat_norm(q.v, q.v);
		subject.setOrientation(q);

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
