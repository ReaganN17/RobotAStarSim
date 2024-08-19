#include "FieldGrid.cpp"

struct PathPoint {
	float x, y;
	float disFromStart;
	bool pivotPoint = false;

	PathPoint* nextPoint = nullptr;

	//vector find purpoeses
	friend bool operator==(const PathPoint& n1, const PathPoint& n2);

	//default constructors
	PathPoint();

	//point constructor
	PathPoint(float x, float y);

	//check if points are on top of each other
	bool isEqualPoint(PathPoint* p);
};

bool operator==(const PathPoint& n1, const PathPoint& n2) {
	return n1.x == n2.x && n1.y == n2.y && n1.pivotPoint == n2.pivotPoint;
}

PathPoint::PathPoint() {}

PathPoint::PathPoint(float x, float y) : x(x), y(y) {}

bool PathPoint::isEqualPoint(PathPoint* p) {
	return p->x == this->x && p->y == this->y;
}

struct AutoPath : Object {
	vector<PathPoint> gridPoints;
	vector<PathPoint> pivotPoints;
	vector<PathPoint> pathPoints;

	FieldObjects* fobjs = nullptr;
	FieldGrid* fg = nullptr;
	
	//default constructor
	AutoPath();

	//constructor with field grid
	AutoPath(FieldGrid* fg);

	//gets distance between two points
	float distancePoint(PathPoint* a, PathPoint* b);

	//gets distance point from the line connecting two other points
	float pointFromLinePP(PathPoint* pp1, PathPoint* pp2, PathPoint* point);

	//checks if line connecting two points crosses any obstacles
	bool lineTouchingAny(PathPoint* a, PathPoint* b);

	//interpolates points on one dimension
	float interpolation(float val0, float val1, float val2, float val3, float t);

	//interpolates 2d points from a path and index
	PathPoint interpolatePoint(vector<PathPoint>* points, int index, float t);

	//creates points from grid
	void createGridPath();

	//creates pivot points from grid points
	void createPivotPath();

	//creates auto path by interpolating from pivot points
	void createAutoPath();

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

float AutoPath::pointFromLinePP(PathPoint* pp1, PathPoint* pp2, PathPoint* point) {
	return pointFromLine(pp1->x, pp1->y, pp2->x, pp2->y, point->x, point->y);
}

bool AutoPath::lineTouchingAny(PathPoint* a, PathPoint* b) {
	if (fobjs == nullptr) return false;

	return fobjs->lineIsTouchingAny(a->x, a->y, b->x, b->y);
}

float AutoPath::interpolation(float val0, float val1, float val2, float val3, float t) {
	t -= (int)t;

	val0 = clampF(val1 - 5, val0, val1 + 5);
	val3 = clampF(val2 - 5, val3, val2 + 5);

	float tt = t * t;
	float ttt = tt * t;

	float q1 = -ttt + 2.f * tt - t;
	float q2 = 3.f * ttt - 5.f * tt + 2.f;
	float q3 = -3.f * ttt + 4.f * tt + t;
	float q4 = ttt - tt;

	return (0.5f * (val0 * q1 + val1 * q2 + val2 * q3 + val3 * q4));
}

PathPoint AutoPath::interpolatePoint(vector<PathPoint>* points, int index, float t) {
	float x = interpolation(points->at(max(0, index - 1)).x, points->at(index).x,
		points->at(min(points->size() - 1, index + 1)).x, points->at(min(points->size() - 1, index + 2)).x, t);

	float y = interpolation(points->at(max(0, index - 1)).y, points->at(index).y,
		points->at(min(points->size() - 1, index + 1)).y, points->at(min(points->size() - 1, index + 2)).y, t);

	return PathPoint(x, y);
}

void AutoPath::createGridPath() {
	gridPoints.clear();

	if (fg->endNode == nullptr || fg->startNode == nullptr) return;

	FieldNode* curNode = fg->endNode;

	if (curNode->parent == nullptr) return;

	while (curNode->parent != nullptr) {
		fg->nodes[curNode->y * fg->gw + curNode->x].path = true;

		gridPoints.push_back(PathPoint(curNode->x * fg->gxscale + 0.5 * fg->gxscale, curNode->y * fg->gyscale + 0.5 * fg->gyscale));

		curNode = curNode->parent;
	}

	gridPoints.push_back(PathPoint(curNode->x * fg->gxscale + 0.5 * fg->gxscale, curNode->y * fg->gyscale + 0.5 * fg->gyscale));

	reverse(gridPoints.begin(), gridPoints.end());
}

void AutoPath::createPivotPath() {
	pivotPoints.clear();

	if (gridPoints.size() == 0) return;

	gridPoints.front().pivotPoint = true;
	gridPoints.back().pivotPoint = true;

	pivotPoints.push_back(PathPoint(gridPoints.front()));
	pivotPoints.push_back(PathPoint(gridPoints.back()));

	int curIndex = 0;

	while (curIndex < pivotPoints.size() - 1) {

		if (lineTouchingAny(&pivotPoints.at(curIndex), &pivotPoints.at(curIndex + 1))) {

			float maxR = -1;
			PathPoint maxPoint = pivotPoints.at(curIndex);
			int pivotCounter = 0;

			for (auto i : gridPoints) {

				if (curIndex == pivotCounter - 1) {

					if (i.pivotPoint) break;

					float res = pointFromLinePP(&pivotPoints.at(curIndex), &pivotPoints.at(curIndex + 1), &i);

					if (maxR < res) {maxR = res; maxPoint = i;}

				}

				if (i.pivotPoint) {pivotCounter++;}
			}

			if (!maxPoint.isEqualPoint(&pivotPoints.at(curIndex)) && !maxPoint.isEqualPoint(&pivotPoints.at(curIndex + 1))) {
				
				find(gridPoints.begin(), gridPoints.end(), maxPoint)->pivotPoint = true;
				maxPoint.pivotPoint = true;

				auto it = pivotPoints.begin();
				advance(it, curIndex + 1);
				pivotPoints.insert(it, maxPoint);

			} else {curIndex++;}

		} else {curIndex++;}

	}
}

void AutoPath::createAutoPath() {
	pathPoints.clear();

	if (pivotPoints.size() == 0) return;

	for (int i = 0; i < pivotPoints.size() - 1; i++) {
		float dis = distancePoint(&pivotPoints.at(i), &pivotPoints.at(i + 1));
		int divisor = 0.1 * dis;


		for (float t = 0; t < 1; t += 1.f / divisor) {
			pathPoints.push_back(interpolatePoint(&pivotPoints, i, t));
			if (t == 0) pathPoints.back().pivotPoint = true;
		}
	}

	pathPoints.push_back(pivotPoints.back());

	for (int i = 0; i < pathPoints.size() - 1; i++) {
		pathPoints.at(i).nextPoint = &pathPoints.at(i + 1);
	}
}

AutoPath& AutoPath::update() {


	return *this;
}

void AutoPath::render() {
	for (auto i : pivotPoints) {
		fg->drawFieldRect(i.x, i.y, 2.5, 2.5, 0x0000ff);
	}

	for (auto i : pathPoints) {
		fg->drawFieldRect(i.x, i.y, 2.5, 2.5, 0xff00ff);
	}
}