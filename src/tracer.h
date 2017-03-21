#pragma once

#include <inttypes.h>
#include "linmath.h"
#include "base.h"
#include "scene.h"
#include "geometries.h"
#include "materials.h"

namespace Tracer {

enum PixelFmt {
	tr_fmt_rgb = 3,
	tr_fmt_luminance = 1,
};

struct BufferInfo {
	PixelFmt    pixel_format;
	unsigned int width, height;
	uint8_t* buffer;
};

int trace(Scene* scene, BufferInfo info);

};
