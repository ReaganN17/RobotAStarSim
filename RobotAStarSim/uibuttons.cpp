union ButtonByte
{
	unsigned char byte = 0;

	struct
	{
		//is it worth it?

		bool down : 1;
		bool pressed : 1;
		bool released : 1;
		bool hover : 1;
		bool stick : 1;
		bool circle : 1;
		bool disable : 1;
		bool erm : 1;
	};
};

struct Button : Object {
	u16 dW, dH;
	ButtonByte bb;

	Button(float x, float y, float w, float h, u8 z, Image* spritesheet, CropInfo crop, bool stick, bool circle);
	Button(float x, float y, float w, float h, const char* filename, u8 z, bool stick, bool circle);
	Button(float x, float y, float w, float h, u32 color, u8 z, bool stick, bool circle);
	Button();

	~Button();

	Button& update();

	bool isMouseOver();
};

Button::~Button() { if (!ob.instance) return; }

Button::Button() {}

//long ass constructor cause i dont feel like being neat
Button::Button(float x, float y, float w, float h, const char* filename, u8 z = 2, bool stick = false, bool circle = false) : Object(x, y, w, h, filename, z) {
	dW = w, dH = h;
	bb.stick = stick;
	bb.circle = circle;
	ob.cameraLinked = false;
}

Button::Button(float x, float y, float w, float h, u8 z, Image* spritesheet, CropInfo crop, bool stick = false, bool circle = false)
	:Object(x, y, w, h, z, spritesheet, crop) 
{
	dW = w, dH = h;
	bb.stick = stick;
	bb.circle = circle;
	ob.cameraLinked = false;
}

Button::Button(float x, float y, float w, float h, u32 color, u8 z = 2, bool stick = false, bool circle = false) : Object(x, y, w, h, color, z) {
	dW = w, dH = h;
	bb.stick = stick;
	bb.circle = circle;
	ob.cameraLinked = false;
}

//default key is left mouse
Button& Button::update() {
	bb.pressed = false;
	bb.released = false;

	bb.hover = isMouseOver();

	if (bb.disable)  bb.hover = false, bb.down = false;

	if (bb.stick) {
		if (bb.hover && pressed(LEFT_MOUSE) && !bb.down) {
			bb.pressed = true;
			bb.down = true;
		}
		else if (bb.hover && pressed(LEFT_MOUSE) && bb.down) {
			bb.released = true;
			bb.down = false;
		}
	}
	else {
		bb.down = false;
		if (bb.hover && pressed(LEFT_MOUSE)) {
			bb.pressed = true;
			bb.erm = true;
		}
		if (bb.erm && bb.hover && isdown(LEFT_MOUSE)) {
			bb.down = true;
		}
		if (bb.erm && bb.hover && released(LEFT_MOUSE)) {
			bb.released = true;
			bb.erm = false;
		}
		if (!bb.hover) {
			bb.erm = false;
		}
	}

	if (bb.down) setShade(50); else { setShade(0); }
	if (bb.hover) setScale(1.3 * dW, 1.3 * dH); else { setScale(dW, dH); }

	return *this;
}

bool Button::isMouseOver() {
	if(bb.circle) return (4 * (mouse.x * mouse.x) / (w * w) + 4 * (mouse.y * mouse.y) / (h * h)) <= 1;
	else return (mouse.x >= x - w / 2 && mouse.x <= x + w / 2) && (mouse.y >= y - h / 2 && mouse.y <= y + h / 2);
}
