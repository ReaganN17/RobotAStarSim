#define ALPHA(rgb) (uint8_t)(rgb >> 24)
#define RED(rgb)   (uint8_t)(rgb >> 16)
#define GREEN(rgb) (uint8_t)(rgb >> 8)
#define BLUE(rgb)  (uint8_t)(rgb)

#define UNMULTIPLY(color, alpha) ((0xFF * color) / alpha)
#define BLEND(back, front, alpha) (u8) (((front * alpha) + (back * (255 - alpha))) / 255)


struct RGBAPixel {
	u8 r = 0, g = 0, b = 0, a = 255;

	RGBAPixel(u8* ImgData, u32 src, u8 size) {
		r = (ImgData[src] & 0xff);
		g = (ImgData[((size > 2) ? 1 : 0) + src] & 0xff);
		b = (ImgData[((size > 2) ? 2 : 0) + src] & 0xff);
		a = (size % 2 == 0) ? ImgData[src + ((size > 2)?3:1)] : 255;
	}

	u32 toHexNoA() {
		return 0 << 24 | r << 16 | g << 8| b;
	}

	u32 toHexOneA(u32 back) {
		if (a == 0) return back;
		return toHexNoA();
	}

	u32 toHexWithA(u32 back, u8 opac = 255) {
		if (a == 0) return back;
		return 0 << 24 | BLEND(back, r, a * opac / 255) << 16 | BLEND(back, g, a * opac / 255) << 8 | BLEND(back, b, a * opac / 255);
	}

	RGBAPixel& shade(u8 opac) {
		r = BLEND(0, r, a * opac / 255);
		g = BLEND(0, g, a * opac / 255);
		b = BLEND(0, b, a * opac / 255);

		return *this;
	}
};