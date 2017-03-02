#pragma once

namespace Tracer {

enum channel_type {
	tr_ui8 = 1;
	tr_f32 = 4;
	tr_f64 = 8;	
};

enum pixel_fmt {
	tr_fmt_rgb;
	tr_fmt_luminance;
};

struct buffer_info {
	channel_type channel_size;
	pixel_fmt    pixel_format;
	unsigned int width, height;
};

}
