#pragma once

namespace Tracer {

enum PixelFmt {
	tr_fmt_rgb = 3,
	tr_fmt_luminance = 1,
};

struct BufferInfo {
	PixelFmt    pixel_format;
	unsigned int width, height;
	void* buffer;
};

}
