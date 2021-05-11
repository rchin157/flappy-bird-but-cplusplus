float render_scale = 0.01f;

void clear_screen(unsigned int colour) {
	unsigned int* pixel = (unsigned int*)renderinfo.memory;
	for (int x = 0; x < renderinfo.width; x++) {
		for (int y = 0; y < renderinfo.height; y++) {
			*pixel++ = colour;
		}
	}
}

void draw_graphic() {

}

void draw_rect_absolute(int x0, int y0, int x1, int y1, unsigned int colour) {
	x0 = clamp(0, renderinfo.width, x0);
	x1 = clamp(0, renderinfo.width, x1);
	y0 = clamp(0, renderinfo.height, y0);
	y1 = clamp(0, renderinfo.height, y1);

	unsigned int* pixel = (unsigned int*)renderinfo.memory + (y0 * renderinfo.width) + x0;
	for (int y = y0; y < y1; y++) {
		for (int x = x0; x < x1; x++) {
			*pixel++ = colour;
		}
		pixel += renderinfo.width - (x1 - x0);
	}
}

void draw_rect_rel_not_centred(float x, float y, float width, float height, unsigned int colour) {
	int scale;
	if (renderinfo.width < renderinfo.height)
		scale = renderinfo.width;
	else
		scale = renderinfo.height;

	x *= scale * render_scale;
	y *= scale * render_scale;
	width *= scale * render_scale;
	height *= scale * render_scale;

	int x0 = (int)x;
	int x1 = (int)(x + width);
	int y0 = (int)y;
	int y1 = (int)(y + height);

	draw_rect_absolute(x0, y0, x1, y1, colour);
}

void draw_rect_relative(float x, float y, float width, float height, unsigned int colour) {
	int scale;
	if (renderinfo.width < renderinfo.height)
		scale = renderinfo.width;
	else
		scale = renderinfo.height;

	x *= scale * render_scale;
	y *= scale * render_scale;
	width *= scale * render_scale;
	height *= scale * render_scale;

	x += renderinfo.width / 2;
	y += renderinfo.height / 2;

	int x0 = (int)(x - (width / 2));
	int x1 = (int)(x + (width / 2));
	int y0 = (int)(y - (height / 2));
	int y1 = (int)(y + (height / 2));

	draw_rect_absolute(x0, y0, x1, y1, colour);
}

void draw_bird(float height) {
	draw_rect_relative(-25, height, 15, 5, 0xFFFFFF);
	draw_rect_relative(-21, height + 1, 1, 1, 0);
	draw_rect_relative(-27, height - 1, 10, 2, 0xFF0000);
}