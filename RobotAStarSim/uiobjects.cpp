struct UIObject : Object {

	UIObject(float x, float y, float w, float h, const char* filename, zLayer z);
	UIObject(float x, float y, float w, float h, zLayer z, Image* spritesheet, CropInfo crop);
	UIObject(float x, float y, float w, float h, u32 c, zLayer z);
	UIObject();
	~UIObject();

};

UIObject::UIObject() {}

UIObject::UIObject(float x, float y, float w, float h, const char* filename, zLayer z = MIDDLE)
	:Object(x, y, w, h, filename, z) {
	ob.cameraLinked = false;
}

UIObject::UIObject(float x, float y, float w, float h, zLayer z, Image* spritesheet, CropInfo crop)
	:Object(x, y, w, h, z, spritesheet, crop) {
	ob.cameraLinked = false;
}

UIObject::UIObject(float x, float y, float w, float h, u32 c, zLayer z = MIDDLE)
	:Object(x, y, w, h, c, z) {
	ob.cameraLinked = false;
}

UIObject::~UIObject() {
	if (!ob.instance) return;
}