#include "FieldGrid.cpp"

struct PathPoint {
	float x, y;
	float disFromEnd;

	PathPoint* nextPoint = nullptr;

	//vector find purpoeses
	friend bool operator==(const PathPoint& n1, const PathPoint& n2);

	//default constructors
	PathPoint();

	//point constructor
	PathPoint(float x, float y);

	PathPoint(float x, float y, float disFromEnd);

	PathPoint(float x, float y, float disFromEnd, PathPoint* nextPoint);

	//check if points are on top of each other
	bool isEqualPoint(PathPoint* p);
};

bool operator==(const PathPoint& n1, const PathPoint& n2) {
	return n1.x == n2.x && n1.y == n2.y;
}

PathPoint::PathPoint() {}

PathPoint::PathPoint(float x, float y) : x(x), y(y) {}

PathPoint::PathPoint(float x, float y, float disFromEnd) : PathPoint(x, y) { this->disFromEnd = disFromEnd; }

PathPoint::PathPoint(float x, float y, float disFromEnd, PathPoint* nextPoint) : PathPoint(x, y, disFromEnd) { this->nextPoint = nextPoint; }

bool PathPoint::isEqualPoint(PathPoint* p) {
	return p->x == this->x && p->y == this->y;
}

struct AutoPath : Object {
	vector<PathPoint> pathPoints;

	FieldObjects* fobjs = nullptr;
	FieldGrid* fg = nullptr;
	
	//default constructor
	AutoPath();

	//constructor with field grid
	AutoPath(FieldGrid* fg);

	//gets distance between two points
	float distancePoint(PathPoint* a, PathPoint* b);

	//gets distance between two points
	float distanceNode(FieldNode* a, FieldNode* b);

	//interpolates points on one dimension
	float interpolation(float val0, float val1, float val2, float val3, float t);

	//interpolates 2d points from a path and index
	PathPoint interpolatePoint(vector<FieldNode*>* points, int index, float t);

	//creates auto path by interpolating from pivot points
	bool createAutoPath();

	//update
	AutoPath& update();

	//render
	void render();

};

AutoPath::AutoPath() {}

AutoPath::AutoPath(FieldGrid* fg) : Object(fg->x, fg->y, fg->w, fg->h, 4), fg(fg), fobjs(fg->fobjs) {}

float AutoPath::distancePoint(PathPoint* a, PathPoint* b) {
	return heuristicRaw(a->x, a->y, b->x, b->y);
}

float AutoPath::distanceNode(FieldNode* a, FieldNode* b) {
	return heuristicRaw(a->x * fg->gxscale + 0.5 * fg->gxscale, a->y * fg->gyscale + 0.5 * fg->gyscale,
		b->x * fg->gxscale + 0.5 * fg->gxscale, b->y * fg->gyscale + 0.5 * fg->gyscale);
}

float AutoPath::interpolation(float val0, float val1, float val2, float val3, float t) {
	t -= (int)t;

	val0 = clampF(val1 - 100, val0, val1 + 100);
	val3 = clampF(val2 - 100, val3, val2 + 100);

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.f * tt - t;
	float q2 = 3.f * ttt - 5.f * tt + 2.f;
	float q3 = -3.f * ttt + 4.f * tt + t;
	float q4 = ttt - tt;

	return (0.5f * (val0 * q1 + val1 * q2 + val2 * q3 + val3 * q4));
}

PathPoint AutoPath::interpolatePoint(vector<FieldNode*>* points, int index, float t) {
	float x = interpolation(
		points->at(max(0, index - 1))->x * fg->gxscale + 0.5 * fg->gxscale, 
		points->at(min(points->size() - 1, index))->x * fg->gxscale + 0.5 * fg->gxscale,
		points->at(min(points->size() - 1, index + 1))->x * fg->gxscale + 0.5 * fg->gxscale,
		points->at(min(points->size() - 1, index + 2))->x * fg->gxscale + 0.5 * fg->gxscale, t);

	float y = interpolation(
		points->at(max(0, index - 1))->y * fg->gyscale + 0.5 * fg->gyscale,
		points->at(min(points->size() - 1, index))->y * fg->gyscale + 0.5 * fg->gyscale,
		points->at(min(points->size() - 1, index + 1))->y * fg->gyscale + 0.5 * fg->gyscale,
		points->at(min(points->size() - 1, index + 2))->y * fg->gyscale + 0.5 * fg->gyscale, t);

	return PathPoint(x, y);
}

bool AutoPath::createAutoPath() {
	pathPoints.clear();

	if (fg->pivots.size() == 0) return false;

	pathPoints.push_back(PathPoint(
		fg->pivots.front()->x * fg->gxscale + 0.5 * fg->gxscale, 
		fg->pivots.front()->y * fg->gyscale + 0.5 * fg->gyscale, 0));

	for (int i = 0; i < fg->pivots.size() - 1; i++) {
		float dis = distanceNode(fg->pivots.at(i), fg->pivots.at(i + 1));
		int divisor = 0.1 * dis;

		for (float t = 0; t < 1; t += 1.f / divisor) {
			PathPoint temp(interpolatePoint(&fg->pivots, i, t));
			temp.nextPoint = &pathPoints.back();
			temp.disFromEnd = distancePoint(&temp, temp.nextPoint) + temp.nextPoint->disFromEnd;

			pathPoints.push_back(temp);
		}
	}

	pathPoints.push_back(PathPoint(fg->pivots.back()->x * fg->gxscale + 0.5 * fg->gxscale, fg->pivots.back()->y * fg->gyscale + 0.5 * fg->gyscale));

	reverse(pathPoints.begin(), pathPoints.end());

	pathPoints.pop_back();

	return true;
}

AutoPath& AutoPath::update() {


	return *this;
}

void AutoPath::render() {

	for (auto i : pathPoints) {
		fg->drawFieldRect(i.x, i.y, 2.5, 2.5, 0xff00ff);
	}
}