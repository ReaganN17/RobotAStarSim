struct Camera : Object {
	float zoom;
	u8 alpha;

	Camera(float x, float y, float zoom);
	~Camera();

	Camera& setPos(float x, float y);
	Camera& setZoom(float zoom);

	Camera& update();
};

Camera::Camera(float x, float y, float zoom) : Object(x, y, 5),  zoom(zoom) {}

Camera::~Camera() {}

Camera& Camera::setPos(float x, float y) {

	this->x = x;
	this->y = y;
	return *this;
}

Camera& Camera::setZoom(float z) {
	zoom = z;

	return *this;
}

Camera& Camera::update() {


	return *this;
}