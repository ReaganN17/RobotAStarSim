//clears ENTIRE screen, used for when window size changes
internal void clearEntireScreen(u32 color) {
	u32* pixel = (u32*)renderWindow.memory;
	for (int y = 0; y < renderWindow.height; y++) {
		for (int x = 0; x < renderWindow.width; x++) {
			*pixel++ = color;
		}
	}
}

//clears screen, used for filler
internal void clearScreen(u32 color) {
	for (int y = 0; y < screenHeight; y++) {
		u32* pixel = (u32*)renderWindow.memory + screenOffset + y * renderWindow.width;
		for (int x = 0; x < screenWidth; x++) {
			*pixel++ = color;
		}
	}
}

//Maintain aspect ratio of 16/9, will add custom ratio later
internal void updateValues() {
	screenWidth = min(renderWindow.height * 16 / 9, renderWindow.width);
	screenHeight = min(renderWindow.width * 9 / 16, renderWindow.height);
	xOffset = max((renderWindow.width - screenWidth) / 2, 0);
	yOffset = max((renderWindow.height - screenHeight) / 2, 0);
	screenOffset = xOffset + yOffset * renderWindow.width;
	render_scale = (float) screenHeight / 540.f;
}

//no matter the size will stretch the image to the boundaries
//image needs to be cropped or sized beforehand

internal void renderStaticBG(const char* file, u8 alpha, u32 background) {
	Image img(file);

	float wscale = ((float)screenWidth/img.w);
	float hscale = ((float)screenHeight/img.h);

	u32 src;

	if (wscale > 0 && hscale > 0) {
		for (int i = 0; i < screenHeight; i++) {
			u32* pixel = (u32*)renderWindow.memory + screenOffset + i * renderWindow.width;
			for (int j = 0; j < screenWidth; j++) {
				if (img.data == NULL) { *pixel++ = background; }
				else {
					src = img.channels * ((int)(j / wscale) + img.w * (int)((screenHeight - i - 1) / hscale));

					RGBAPixel curPixel(img.data, src, img.channels);

					if (alpha == 255) { *pixel++ = curPixel.toHexNoA(); }
					else { *pixel++ = curPixel.toHexWithA(0, alpha); }
				}
			}
		}
	}
}

//render current BG that i use
internal void renderBG() {
	for (int i = 0; i < screenHeight; i++) {
		u32* pixel = (u32*)renderWindow.memory + screenOffset + i * renderWindow.width;
		for (int x = 0; x < screenWidth; x++) {
			*pixel++ = x;
		}
	}
}


//renders a rectangle
internal void renderRect(float x, float y, float half_size_x, float half_size_y, u32 color, u8 alpha = 255, u8 scale = 0, u32 shader = 0) {

	x *= render_scale;
	y *= render_scale;

	half_size_x *= render_scale;
	half_size_y *= render_scale;

	x += screenWidth / 2.f;
	y += screenHeight / 2.f;

	// Change to pixel
	int x0 = clamp(0, x - half_size_x, screenWidth);
	int x1 = clamp(0, x + half_size_x, screenWidth);
	int y0 = clamp(0, y - half_size_y, screenHeight);
	int y1 = clamp(0, y + half_size_y, screenHeight);

	for (int i = y0; i < y1; i++) {
		u32* pixel = (u32*)renderWindow.memory + x0 + screenOffset + i * renderWindow.width;
		for (int j = x0; j < x1; j++) {
			*pixel++ = color;
		}
	}
}

internal void renderImageV2(Image* img, float x, float y, float w, float h, u8 shade = 0) {

	int size_x = w * render_scale;
	int size_y = h * render_scale;

	float wscale = ((float) size_x) / img->w;
	float hscale = ((float) size_y) / img->h;

	int x0 = (x * render_scale) + 0.5 * (screenWidth - size_x);
	int y0 = (y * render_scale) + 0.5 * (screenHeight - size_y);

	int offsetRight = max(0, x0 + size_x - screenWidth);
	int offsetLeft = max(0, -x0);

	int offsetBottom = max(0, -y0);
	int offsetTop = max(0, y0 + size_y - screenHeight);

	u32 src = 0;
	
	if (wscale > 0 && hscale > 0) {
		for (int i = offsetTop; i < size_y - offsetBottom; i++) {
			u32* pixel = (u32*)renderWindow.memory + x0 + offsetLeft + screenOffset + ((size_y - 1) - i + y0) * renderWindow.width;
			for (int j = offsetLeft; j < size_x - offsetRight; j++) {

				src = img->channels * ((int)(j / wscale) + img->w * (int)(i / hscale));
				
				RGBAPixel curPixel(img->data, src, img->channels);

				if (shade != 0) { curPixel.shade(255- shade); }
				
				*pixel++ = curPixel.toHexOneA(*pixel);
			}
		}
	}
}

internal void renderRepeatMap(const char*filename, int x, int y, int w, int h) {
	Image img(filename);

	int size_x = w * render_scale;
	int size_y = h * render_scale;

	float wscale = (float)size_x / img.w;
	float hscale = (float)size_y / img.h;

	for (int k = ((x + 480) % w) - 480 - w; k <= 480 + w * 0.5; k += w) {
		for (int l = ((y + 270) % h) - 270 - h; l <= 270 + h * 0.5; l += h) {

			int x0 = (k) * render_scale + 0.5 * (screenWidth - size_x);
			int y0 = (l) * render_scale + 0.5 * (screenHeight - size_y);

			int offsetRight = max(0, x0 + size_x - screenWidth);
			int offsetLeft = max(0, -x0);

			int offsetBottom = max(0, -y0);
			int offsetTop = max(0, y0 + size_y - screenHeight);

			u32 src = 0;

			if (wscale > 0 && hscale > 0) {
				for (int i = offsetTop; i < size_y - offsetBottom; i++) {
					u32* pixel = (u32*)renderWindow.memory + x0 + offsetLeft + screenOffset + ((size_y - 1) - i + y0) * renderWindow.width;
					for (int j = offsetLeft; j < size_x - offsetRight; j++) {
						src = img.channels * ((int)(j / wscale) + img.w * (int)(i / hscale));

						RGBAPixel curPixel(img.data, src, img.channels);

						*pixel++ = curPixel.toHexOneA(*pixel);
					}
				}
			}
		}
	}
}


//used for debugging, will delete soon
internal void
draw_number(int number, float x, float y, float size) {
	float half_size = size * .5f;
	u32 color = 0xff0000;
	float alpha = 1;


	bool drew_number = false;
	while (number || !drew_number) {
		drew_number = true;

		int digit = number % 10;
		number = number / 10;

		switch (digit) {
		case 0: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 1: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			x -= size * 2.f;
		} break;

		case 2: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x, y, 1.5f * size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x + size, y + size, half_size, half_size, color, alpha);
			renderRect(x - size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 3: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x - half_size, y, size, half_size, color, alpha);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			x -= size * 4.f;
		} break;

		case 4: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - size, y + size, half_size, 1.5f * size, color, alpha);
			renderRect(x, y, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 5: {
			renderRect(x, y + size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x, y, 1.5f * size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, 1.5f * size, half_size, color, alpha);
			renderRect(x - size, y + size, half_size, half_size, color, alpha);
			renderRect(x + size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 6: {
			renderRect(x + half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x + half_size, y, size, half_size, color, alpha);
			renderRect(x + half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y - size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 7: {
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 8: {
			renderRect(x - size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x, y + size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y - size * 2.f, half_size, half_size, color, alpha);
			renderRect(x, y, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;

		case 9: {
			renderRect(x - half_size, y + size * 2.f, size, half_size, color, alpha);
			renderRect(x - half_size, y, size, half_size, color, alpha);
			renderRect(x - half_size, y - size * 2.f, size, half_size, color, alpha);
			renderRect(x + size, y, half_size, 2.5f * size, color, alpha);
			renderRect(x - size, y + size, half_size, half_size, color, alpha);
			x -= size * 4.f;
		} break;
		}

	}
}

