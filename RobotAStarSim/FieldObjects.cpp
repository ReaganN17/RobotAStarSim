struct FieldObject {
	bool obstacle = false;

	float x, y, w = -1, h = -1, r = -1;

	//Default Constructor
	FieldObject();

	//Point Contstructor
	FieldObject(float x, float y);

	//Rect Constructor
	FieldObject(float x, float y, float w, float h);

	//Circle Constructor
	FieldObject(float x, float y, float r);

	//point is touching obstacle (if obstacle is true)
	bool isTouching(float x, float y);

	//line is touching obstacle (if obstacle is true)
	bool lineIsTouching(float x0, float y0, float x1, float y1);
};

FieldObject::FieldObject() {}

FieldObject::FieldObject(float x, float y) : x(x), y(y) {}

FieldObject::FieldObject(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

FieldObject::FieldObject(float x, float y, float r) : x(x), y(y), r(r) {}

bool FieldObject::isTouching(float x, float y) {
	if (!obstacle) return false;

	if (r != -1) {
		return (x - this->x) * (x - this->x) + (y - this->y) * (y - this->y) <= r * r;
	}
	else if (w != -1 && h != -1) {
		return (this->x - 0.5 * w <= x && x <= this->x + 0.5 * w && this->y - 0.5 * h <= y && y <= this->y + 0.5 * h);
	}
	else {
		return (this->x == x && this->y == y);
	}

	return false;
}

bool FieldObject::lineIsTouching(float x0, float y0, float x1, float y1) {
	if (!obstacle) return false;

	if (r != -1) {
		return lineIntersectCircle(x0, y0, x1, y1, x, y, r);
	}
	else if (w != -1 && h != -1) {
		return lineIntersectRect(x0, y0, x1, y1, x, y, w, h);
	}
	else {
		return pointOnLine(x, y, x0, y0, x1, y1);
	}

	return false;
}


struct FieldObjects {
	vector<FieldObject*> fieldObjs;

	//default constructor
	FieldObjects();

	//add field object (only once)
	void add(FieldObject* fobj);

	//remove field object (only if its in it)
	void remove(FieldObject* fobj);

	//checks if point is touching any (with obstacle enabled)
	bool pointIsTouchingAny(float x, float y);

	//checks if line is touching any (with obstacle enabled)
	bool lineIsTouchingAny(float x0, float y0, float x1, float y1);
};

FieldObjects::FieldObjects() {}

void FieldObjects::add(FieldObject* fobj) {
	auto i = find(fieldObjs.begin(), fieldObjs.end(), fobj);
	if (i != fieldObjs.end()) return;

	fieldObjs.push_back(fobj);
}

void FieldObjects::remove(FieldObject* fobj) {
	auto i = find(fieldObjs.begin(), fieldObjs.end(), fobj);
	if (i == fieldObjs.end()) return;
	
	fieldObjs.erase(i);
}

bool FieldObjects::pointIsTouchingAny(float x, float y) {
	for (auto i : fieldObjs) {
		if ((i)->isTouching(x, y)) return true;
	}

	return false;
}

bool FieldObjects::lineIsTouchingAny(float x0, float y0, float x1, float y1) {
	for (auto i : fieldObjs) {
		if ((i)->lineIsTouching(x0, y0, x1, y1)) return true;
	}

	return false;
}