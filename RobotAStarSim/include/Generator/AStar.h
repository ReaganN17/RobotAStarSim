#pragma once

#include <vector>
#include "../Imports/Translation2d.h"

struct Node {
	short x = -1, y = -1;
	bool obstacle = false, visited = false;
	float fGlobalGoal = FLT_MAX, fLocalGoal = FLT_MAX;

	std::vector<Node*> neighbours;

	Node* parent = nullptr;

	Node();
	Node(short x, short y);

	void addNeighbour(Node* other);
	void resetGoals();
};

struct Grid {
	Node* nodes = nullptr;
	short length = 0, width = 0;
	float x_scale = 1, y_scale = 1;
	Node* startNode = nullptr;
	Node* endNode = nullptr;

	Grid();
	Grid(float length, float width, float sample_size);

	void update_fo();
	bool pathFind(Node* start, Node* end);

	Node* getNode(short x, short y);

	void createGrid(short l, short w);
	float heuristic(Node* a, Node* b);
	
	Translation2d node_to_t2d(Node* node);
	Node* t2d_to_node(Translation2d);
};
