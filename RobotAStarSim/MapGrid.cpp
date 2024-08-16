

struct Node {
	bool bObstacle = false;
	bool bVisited = false;
	bool path = false;
	float fGlobalGoal;
	float fLocalGoal;
	u16 x, y;
	vector <Node*> vecNeighbours;
	
	Node* parent;
};

struct Obstacle {
	bool rect; //or circle

	short id = -1;

	float x, y, w, h, r; //according to grid

	Obstacle() {}

	Obstacle(float x, float y, float w, float h, short id) : x(x), y(y), w(w), h(h), id(id) { r = 0;  rect = true; }

	Obstacle(float x, float y, float r, short id):  x(x), y(y), r(r), id(id){ w = 0; h = 0; rect = false; }

	bool isTouching(float x, float y) {
		if (rect) {
			return (this->x - w * 0.5 <= x && x <= this->x + w * 0.5 && this->y - h * 0.5 <= y && y <= this->y + h * 0.5);
		}
		else {
			return (x - this->x) * (x - this->x) + (y - this->y) * (y - this->y) <= r * r;
		}
	}

	bool isValue(float x, float y, float w, float h, float r, bool rect) {
		if (this->x == x && this->y == y) {
			if (rect) {
				return (this->w == w && this->h == h);
			}
			else {
				return (this->r == r);
			}
		}

		return false;
	}

	bool lineTouching(float x0, float y0, float x1, float y1) {
		if (rect) {
			return linesIntersect(x0, y0, x1, y1, x - w * 0.5, y - h * 0.5, x + w * 0.5, y - h * 0.5) ||
			linesIntersect(x0, y0, x1, y1, x + w * 0.5, y - h * 0.5, x + w * 0.5, y + h * 0.5) ||
			linesIntersect(x0, y0, x1, y1, x + w * 0.5, y + h * 0.5, x - w * 0.5, y + h * 0.5) ||
			linesIntersect(x0, y0, x1, y1, x - w * 0.5, y + h * 0.5, x - w * 0.5, y - h * 0.5);
		}
		else {
			return lineIntersectCircle(x0, y0, x1, y1, x, y, r);
		}

		return false;
	}
};

struct Obstacles {
	short nextID = 0;

	vector<Obstacle> obstacles;

	Obstacles() {}

	void addRect(float x, float y, float w, float h) {
		obstacles.push_back(Obstacle(x, y, w, h, nextID));
		nextID++;
	}

	void addCircle(float x, float y, float r) {
		obstacles.push_back(Obstacle(x, y, r, nextID));
		nextID++;
	}

	void addPoint(float x, float y) {
		obstacles.push_back(Obstacle(x, y, 0, nextID));
		nextID++;
	}

	void removeID(short id) {
		for (vector<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
			if (it->id == id) { obstacles.erase(it); break; }
		}
	}

	bool isTouchingAny(float x, float y) {
		for (auto i : obstacles) {
			if (i.isTouching(x, y)) return true;
		}

		return false;
	}

	Obstacle* getObject(float x, float y) {
		for (auto i : obstacles) {
			if (i.isTouching(x, y)) return &i;
		}

		return nullptr;
	}

	Obstacle* getObjectID(short id) {
		for (auto i : obstacles) {
			if (i.id == id) return &i;
		}

		return nullptr;
	}

	bool lineTouchingAny(float x0, float y0, float x1, float y1) {
		for (auto i : obstacles) {
			if (i.lineTouching(x0, y0, x1, y1)) return true;
		}

		return false;
	}
};

struct MapGrid : Object {
	Node* nodes = nullptr;
	u16 gw = 0, gh = 0;

	float dx = 0, dy = 0;
	float gridxscale;
	float gridyscale;

	bool pathUpdated = false;

	Node* nodeStart = nullptr;
	Node* nodeEnd = nullptr;

	Obstacles* obs;

	MapGrid() {}

	Node*& setStart(int x, int y) {
		x = clamp(0, x, gw - 1);
		y = clamp(0, y, gh - 1);

		nodeStart = &nodes[x + y * gw];

		pathUpdated = true;

		return nodeStart;
	}

	//set goal node
	Node*& setEnd(int x, int y) {
		x = clamp(0, x, gw - 1);
		y = clamp(0, y, gh - 1);

		nodeEnd = &nodes[x + y * gw];

		pathUpdated = true;

		return nodeEnd;
	}

	MapGrid& createNeighbors(u8 range) {
		for (int i = 1; i <= range; i++) {
			for (u16 x = 0; x < gw; x++) {
				for (u16 y = 0; y < gh; y++)
				{
					if (y > (i - 1))
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - i) * gw + (x + 0)]);
					if (y < gh - i)
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + i) * gw + (x + 0)]);
					if (x > (i - 1))
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x - i)]);
					if (x < gw - i)
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + 0) * gw + (x + i)]);

					if (y > (i - 1) && x > (i - 1))
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - i) * gw + (x - i)]);
					if (y > (i - 1) && x < gw - i)
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y - i) * gw + (x + i)]);
					if (y < gh - i && x < gw - i)
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + i) * gw + (x + i)]);
					if (y < gh - i && x > (i - 1))
						nodes[y * gw + x].vecNeighbours.push_back(&nodes[(y + i) * gw + (x - i)]);
				}
			}
		}



		return *this;
	}

	MapGrid& createEmpty(int w, int h) {
		if (nodes != nullptr) delete[] nodes;

		gw = w;
		gh = h;

		nodes = new Node[gw * gh];

		//initlize nodes
		for (u16 x = 0; x < gw; x++) {
			for (u16 y = 0; y < gh; y++)
			{
				nodes[y * gw + x].x = x;
				nodes[y * gw + x].y = y;
				nodes[y * gw + x].bObstacle = false;
				nodes[y * gw + x].path = false;
				nodes[y * gw + x].parent = nullptr;
				nodes[y * gw + x].bVisited = false;
			}
		}

		//create neighbors
		createNeighbors(1);

		return *this;
	}

	MapGrid& createObstacles() {

		for (int i = 0; i < gw * gh; i++) {
			nodes[i].bObstacle = obs->isTouchingAny(nodes[i].x, nodes[i].y);
		}

		return *this;
	}

	

	float heuristic(Node* a, Node* b) {
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	}

	Node*& reset() {
		for (u16 x = 0; x < gw; x++) {
			for (u16 y = 0; y < gh; y++) {
				nodes[y * gw + x].bVisited = false;
				nodes[y * gw + x].fGlobalGoal = INFINITY;
				nodes[y * gw + x].fLocalGoal = INFINITY;
				nodes[y * gw + x].parent = nullptr;
				nodes[y * gw + x].path = false;
			}
		}
		return nodes;
	}

	Node*& drawPath() {
		Node* curNode = nodeEnd;

		while (curNode->parent != nullptr) {
			nodes[curNode->y * gw + curNode->x].path = true;

			curNode = curNode->parent;
		}

		return nodes;
	}
	

	bool createPath() {
		reset();

		//start at the started node
		Node* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		//create list of explored but untested nodes
		list<Node*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd) //first path found is the path that is taken
		//while (!listNotTestedNodes.empty())
		{
			// sort by lowest heurisitc
			listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// clear notTestNodes of any visited nodes
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			//if non left in the list break
			if (listNotTestedNodes.empty())
				break;

			//check node as visited (because we are visiting them)
			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true;

			//go through each of the nodes neighbors (i didnt wanna do diagnol)
			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				//obstacle check
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				//calculate distance (and if lower distance to current distance)
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + heuristic(nodeCurrent, nodeNeighbour);

				// update distance if it was lower
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// pou16 the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.

					//yea what that guy said ^ (credit to javidx9 on Youtube for helping me learn and code A*)
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}
		}

		return nodeEnd->parent != nullptr;
	}

	MapGrid(Object* map, int xw, int yh, Obstacles* obs) : Object(map->x, map->y, map->w, map->h, 3) {

		gridxscale = w / xw;
		gridyscale = h / yh;

		this->obs = obs;

		dx = x - w * 0.5, dy = y - h * 0.5;

		gw = xw, gh = yh;

		createEmpty(gw, gh);
	}

	void drawGridNode(float x, float y, float scale, u32 color) {
		renderRect(dx + gridxscale * x + gridxscale * 0.5, dy + gridyscale * y + gridyscale * 0.5, gridxscale * 0.5 * scale, gridyscale * 0.5 * scale, color);
	}

	MapGrid& update() {
		if (isdown(SHIFT) && pressed(LEFT_MOUSE)) {
			setStart((mouse.x - gridxscale * 0.5 - dx)/gridxscale, (mouse.y - gridyscale * 0.5 - dy)/gridyscale);
		}

		if (isdown(CTRL) && pressed(LEFT_MOUSE)) {
			setEnd((mouse.x - gridxscale * 0.5 - dx) / gridxscale, (mouse.y - gridyscale * 0.5 - dy) / gridyscale);
		}


		if (pathUpdated) {
			createObstacles();
			createPath();
			drawPath();

			pathUpdated = false;
		}


		return *this;
	}

	void render() {
		for (int i = 0; i < gw; i++) {
			for (int h = 0; h < gh; h++) {
				if (nodes[h * gw + i].path) drawGridNode(i, h, 0.5, 0xffffff);
				if (nodes[h * gw + i].bObstacle) drawGridNode(i, h, 0.5, 0x8f8f8f);
			}
		}

		renderRect(dx + gridxscale * nodeStart->x + gridxscale * 0.5, dy + gridyscale * nodeStart->y + gridyscale * 0.5, gridxscale * 0.5, gridyscale * 0.5, 0x00ff00);
		renderRect(dx + gridxscale * nodeEnd->x + gridxscale * 0.5, dy + gridyscale * nodeEnd->y + gridyscale * 0.5, gridxscale * 0.5, gridyscale * 0.5, 0xff0000);
	}
};

#include "waypoints.cpp"