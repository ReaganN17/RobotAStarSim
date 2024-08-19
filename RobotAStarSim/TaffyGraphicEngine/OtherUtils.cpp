enum MoveTypes {
	LINEAR, INQUAD, OUTQUAD, INOUTQUAD, INCUBE, OUTCUBE, INOUTCUBE,
};

struct Camera {
	int x, y;
	float zoom;
	u8 alpha;

	Camera(int x, int y, float zoom);
	~Camera();

	Camera& setPos(int x, int y);
	Camera& setZoom(float zoom);



	Camera& update(float dt);
};

Camera::Camera(int x, int y, float zoom): x(x), y(y), zoom(zoom) {}

Camera::~Camera() {}

Camera& Camera::setPos(int x, int y) {

	this->x = x;
	this->y = y;
	return *this;
}

Camera& Camera::setZoom(float z) {
	zoom = z;

	return *this;
}

Camera& Camera::update(float dt) {

}
