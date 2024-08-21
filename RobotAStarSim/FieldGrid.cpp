#include "FieldObjects.cpp"

struct FieldNode {
	bool obstacle = false;
	bool visited = false;
	float fGlobalGoal;
	float fLocalGoal;
	u16 x, y;
	vector <FieldNode*> vecNeighbours;

	FieldNode* parent;
};

struct FieldGrid : Object {
	FieldNode* nodes = nullptr;
	u16 gw, gh;
	float gxscale, gyscale, scale;
	float dx = 0, dy = 0;

	FieldNode* startNode = nullptr;
	FieldNode* endNode = nullptr;

	FieldObjects* fobjs = nullptr;

	vector<FieldNode*> pivots;

	//Default Constructor
	FieldGrid();

	//units are in inches (im sorry) (no im not)
	FieldGrid(const char* file, float px, float py, float winch, float hinch, float pscale, float sampleSize, FieldObjects* fobjs);

	//create Empty Grid with dimensions
	void createEmpty(int w, int h);

	//creates Grid With Obstacles
	void createWObstacles(int w, int h);

	//updates obstacles on Grid
	void updateObstacles();

	//set start point
	void setStart(int x, int y);

	//set end point
	void setEnd(int x, int y);

	//set Start and End to nullptr
	void resetEnds();

	//reset Grid
	void reset();

	//heuristic
	float heuristic(FieldNode* a, FieldNode* b);

	//generate path, return true if path made
	bool pathFind();

	//generates pivots (connections where obstacles do not touch)
	bool createPivots();

	float lineTouchingAnyFF(FieldNode* l1, FieldNode* l2);
	
	float pointFromLineFF(FieldNode* l1, FieldNode* l2, FieldNode* point);

	//render rectangles and field
	void render();

	//draws based on Grid Scale
	void drawGridNode(float x, float y, float scale, u32 color);

	//draws based on field scale
	void drawFieldRect(float x, float y, float w, float h, u32 color);
};

FieldGrid::FieldGrid() {}

FieldGrid::FieldGrid(const char* file, float px, float py, float winch, float hinch, float scale, float sampleSize, FieldObjects* fobjs)
	: Object(px, py, winch * scale, hinch * scale, file, 2), scale(scale), fobjs(fobjs) {
	//rounds sample Sizes to produce whole number grid sizes

	dx = x - w * 0.5, dy = y - h * 0.5;

	gxscale = clamp(0.1, sampleSize, winch);
	gyscale = clamp(0.1, sampleSize, hinch);
	
	gw = (int)(winch / gxscale);
	gh = (int)(hinch / gyscale);

	gxscale = winch / gw;
	gyscale = hinch / gh; 

	createWObstacles(gw, gh);

	ob.cameraLinked = false;
}


void FieldGrid::createEmpty(int w, int h) {
	if (nodes != nullptr) delete[] nodes;

	w = max(0, w), h = max(0, h);

	gw = w, gh = h;

	nodes = new FieldNode[gw * gh];

	//initlize nodes
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			nodes[y * gw + x].x = x;
			nodes[y * gw + x].y = y;
			nodes[y * gw + x].obstacle = false;
			nodes[y * gw + x].parent = nullptr;
			nodes[y * gw + x].visited = false;
		}
	}

	//create Neighbours
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			//cardinal directions
			if (y > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x + 0)]);
			if (y < gh - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x + 0)]);
			if (x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x - 1)]);
			if (x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x + 1)]);

			//diagnal direcitons
			if (y > 0 && x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x - 1)]);
			if (y > 0 && x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x + 1)]);
			if (y < gh - 1 && x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x + 1)]);
			if (y < gh - 1 && x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x - 1)]);
		}
	}
}

void FieldGrid::createWObstacles(int w, int h) {
	if (nodes != nullptr) delete[] nodes;

	w = max(0, w), h = max(0, h);

	gw = w, gh = h;

	nodes = new FieldNode[gw * gh];

	//initlize nodes
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			nodes[y * gw + x].x = x;
			nodes[y * gw + x].y = y;
			nodes[y * gw + x].obstacle = fobjs->pointIsTouchingAny(x * gxscale + 0.5 * gxscale, y * gyscale + 0.5 * gyscale);
			nodes[y * gw + x].parent = nullptr;
			nodes[y * gw + x].visited = false;
			
			
		}
	}

	//create Neighbours
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++)
		{
			//cardinal directions
			if (y > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x + 0)]);
			if (y < gh - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x + 0)]);
			if (x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x - 1)]);
			if (x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x + 1)]);

			//diagnal direcitons
			if (y > 0 && x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x - 1)]);
			if (y > 0 && x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - 1) * gw + (x + 1)]);
			if (y < gh - 1 && x < gw - 1)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x + 1)]);
			if (y < gh - 1 && x > 0)
				nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 1) * gw + (x - 1)]);
		}
	}
}

void FieldGrid::updateObstacles() {
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++) {
			nodes[y * gw + x].obstacle = false;
			nodes[y * gw + x].obstacle = fobjs->pointIsTouchingAny(x * gxscale + 0.5 * gxscale, y * gyscale + 0.5 * gyscale);
		}
	}
}

void FieldGrid::setStart(int x, int y) {
	x = clamp(0, x, gw - 1);
	y = clamp(0, y, gh - 1);


	startNode = &nodes[y * gw + x];
}

void FieldGrid::setEnd(int x, int y) {
	x = clamp(0, x, gw - 1);
	y = clamp(0, y, gh - 1);


	endNode = &nodes[y * gw + x];
}

void FieldGrid::resetEnds() {
	startNode = nullptr;
	endNode = nullptr;
}

void FieldGrid::reset() {
	for (u16 x = 0; x < gw; x++) {
		for (u16 y = 0; y < gh; y++) {
			nodes[y * gw + x].visited = false;
			nodes[y * gw + x].fGlobalGoal = INFINITY;
			nodes[y * gw + x].fLocalGoal = INFINITY;
			nodes[y * gw + x].parent = nullptr;
		}
	}
}

float FieldGrid::heuristic(FieldNode* a, FieldNode* b) {
	return heuristicRaw(a->x, a->y, b->x, b->y);
}

bool FieldGrid::pathFind() {
	reset();

	if (startNode == nullptr || endNode == nullptr) return false;

	//start at the started node
	FieldNode* curNode = startNode;
	startNode->fLocalGoal = 0.0f;
	startNode->fGlobalGoal = heuristic(startNode, endNode);

	//create list of explored but untested nodes
	list<FieldNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(startNode);

	while (!listNotTestedNodes.empty() && curNode != endNode) //first path found is the path that is taken
		//while (!listNotTestedNodes.empty())
	{
		// sort by lowest heurisitc
		listNotTestedNodes.sort([](const FieldNode* lhs, const FieldNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// clear notTestNodes of any visited nodes
		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		//if non left in the list break
		if (listNotTestedNodes.empty())
			break;

		//check node as visited (because we are visiting them)
		curNode = listNotTestedNodes.front();
		curNode->visited = true;

		//go through each of the nodes neighbors (i didnt wanna do diagnol)
		for (auto nodeNeighbour : curNode->vecNeighbours)
		{
			//obstacle check
			if (!nodeNeighbour->visited && nodeNeighbour->obstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			//calculate distance (and if lower distance to current distance)
			float fPossiblyLowerGoal = curNode->fLocalGoal + heuristic(curNode, nodeNeighbour);

			// update distance if it was lower
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = curNode;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// pou16 the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.

				//yea what that guy said ^ (credit to javidx9 on Youtube for helping me learn and code A*)
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, endNode);
			}
		}
	}

	return endNode->parent != nullptr;
}

bool FieldGrid::createPivots() {
	pivots.clear();

	if (endNode == nullptr || endNode->parent == nullptr) return false;

	pivots.push_back(endNode);
	pivots.push_back(startNode);

	FieldNode* curNode = nullptr;
	FieldNode* pivotNode = nullptr;
	int curIndex = 0;

	while (curIndex < pivots.size() - 1) {

		curNode = pivots.at(curIndex)->parent;

		if (lineTouchingAnyFF(pivots.at(curIndex), pivots.at(curIndex + 1))) {

			float maxD = -1;

			while (curNode != pivots.at(curIndex + 1)) {

				float d = pointFromLineFF(pivots.at(curIndex), pivots.at(curIndex + 1), curNode);

				if (maxD < d) {
					maxD = d;
					pivotNode = curNode;
				}

				curNode = curNode->parent;
			}

			if (pivotNode != nullptr) {
				pivots.insert(pivots.begin() + curIndex + 1, pivotNode);
				pivotNode = nullptr;
			}
			else {
				curIndex++;
			}

		}
		else {
			curIndex++;
		}


	}

	return true;
}

float FieldGrid::pointFromLineFF(FieldNode* l1, FieldNode* l2, FieldNode* point) {
	return pointFromLine(l1->x, l1->y, l2->x, l2->y, point->x, point->y);
}

float FieldGrid::lineTouchingAnyFF(FieldNode* l1, FieldNode* l2){
	return fobjs->lineIsTouchingAny(
		l1->x * gxscale + 0.5 * gxscale,
		l1->y * gyscale + 0.5 * gyscale,
		l2->x * gxscale + 0.5 * gxscale,
		l2->y * gyscale + 0.5 * gyscale);
}

void FieldGrid::render() {
	Object::render();

	if (startNode != nullptr) { drawGridNode(startNode->x, startNode->y, 1, 0x00ff00); }
	if (endNode != nullptr) { drawGridNode(endNode->x, endNode->y, 1, 0xff0000); }

	for (int i = 0; i < gw; i++) {
		for (int h = 0; h < gh; h++) {
			if (nodes[h * gw + i].obstacle) drawGridNode(i, h, 0.5, 0x8f8f8f);
		}
	}

	FieldNode* curNode = endNode;
	while (curNode != nullptr) {

		drawGridNode(curNode->x, curNode->y, 0.5, 0xffffff);

		curNode = curNode->parent;
	}

	for (auto i : pivots) {
		drawGridNode(i->x, i->y, 0.5, 0x0000ff);
	}

	
}

void FieldGrid::drawGridNode(float x, float y, float scale, u32 color) {
	drawFieldRect(x * gxscale + gxscale * 0.5, y * gyscale + gyscale * 0.5, gxscale * scale, gyscale * scale, color);
}

void FieldGrid::drawFieldRect(float x, float y, float w, float h, u32 color) {
	renderRect(dx + x * this->scale, dy + y * this->scale, this->scale * w * 0.5, this->scale * h * 0.5, color);
}