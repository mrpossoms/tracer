#include <math.h>
#include <unistd.h>
#include <ncurses.h>
#include "tracer.h"

using namespace Tracer;

bool USE_CURSES = true;

int proc_opts(int argc, char* argv[])
{
	while(1)
	{

		int c = getopt(argc, argv, "n");
		if(c == -1) break;

		switch(c)
		{
			case 'n':
				USE_CURSES = false;
				break;
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	BufferInfo info = {
		.pixel_format = tr_fmt_rgb,
	};

	proc_opts(argc, argv);

	info.debug = !USE_CURSES;	

	Light light0;
	Light light1(Vec3(0.25, 0.25, 0.25));
	Plastic plastic;
	BlackBody blackbody;

	// Sphere subject(Vec3(0, 0, 4), 1);
	Plane subject(Vec3(0, 0, 4), Vec3(-0.75f, -0.75f, 0), Vec3(0.75f, 0.75f, 0));
	Sphere sphere0(Vec3(0, -4, 0), 2);
	Sphere sphere1(Vec3(0, 0, 80), 70);

	subject.material = &plastic;
	sphere0.material = &light0;
	sphere1.material = &light1;

	Surface* surfaces[] = { &subject, &sphere1, &sphere0, NULL,};
	Scene scene = {
		.view = Viewer(1.75, 2, M_PI / 2, 1),
		.surfaces = surfaces,
	};

	initscr(); 

	if(USE_CURSES)
	{
		cbreak(); noecho();
	}

	float t = 0;
	Quat w, x, y, q;

	x.from_axis_angle(1, 0, 0, 0.01);
	y.from_axis_angle(0, 0, 1, 0.01);
	w.from_axis_angle(0, 1, 0, 0.01);

	while(1)
	{
		getmaxyx(stdscr, info.height, info.width);
		uint8_t buf[info.width * info.height * 3];
		info.buffer = buf;

		const float r = 8;
		sphere0.position.x = cos(t) * r;
		sphere0.position.y = 2;
		sphere0.position.z = sin(t) * r + 4;
		t += 0.01;
		q = q * w;
		q = q * y;
		subject.setOrientation(q);

		mat4x4 view;
		Vec3 center(cosf(t) * 0.5, sinf(t) * 0.5, 1);
		mat4x4_look_at(view, VEC3_ZERO.v, center.v, VEC3_UP.v);	

		scene.view.view(view);
		trace(&scene, info);

		if(USE_CURSES)
		{
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
		}
		// usleep(1);
	}

	return 0;
}
