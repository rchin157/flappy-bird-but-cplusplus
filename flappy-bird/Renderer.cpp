class Drawable{

};

void render_background() {
	unsigned int* pixel = (unsigned int*)renderinfo.memory;
	for (int x = 0; x < renderinfo.width; x++) {
		for (int y = 0; y < renderinfo.height; y++) {
			*pixel++ = 0x87ceeb;
		}
	}
}

void clear_screen(unsigned int colour) {
	unsigned int* pixel = (unsigned int*)renderinfo.memory;
	for (int x = 0; x < renderinfo.width; x++) {
		for (int y = 0; y < renderinfo.height; y++) {
			*pixel++ = colour;
		}
	}
}

void draw_graphic(Drawable drw) {

}

void draw_rect(int x0, int y0, int x1, int y1, unsigned int colour) {
	if (x0 < 0) x0 = 0;
	if (y0 < 0) y0 = 0;
	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	unsigned int* pixel = (unsigned int*)renderinfo.memory + (y0 * renderinfo.width) + x0;
	for (int y = y0; y < y1; y++) {
		for (int x = x0; x < x1; x++) {
			*pixel++ = colour;
		}
		pixel += renderinfo.width - (x1 - x0);
	}
}