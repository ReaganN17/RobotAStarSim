#pragma once

#include <vector>

struct Node {
	int x, y;
	bool obstacle = false, visited = false;
	double fGlobalGoal, fLocalGoal;

	std::vector<Node*> neighbours;

	Node* parent = nullptr;

	Node();
	Node(int x, int y);

	void addNeighbour(Node* other);
	void resetGoals();
	bool neighboursObstacle();
};

struct Node;

struct Grid {
	Node* nodes = nullptr;

	int gridLength, gridWidth;
	double gridxScale, gridyScale;
	Node* startNode = nullptr;
	Node * endNode = nullptr;

	Grid();
	Grid(double length, double width, double sample_size);
	~Grid();

	void updateGrid();

	bool pathFind(Node* start, Node* end);

	Node* getNode(int x, int y);

	void createGrid(int l, int w);
	double heuristic(Node* a, Node* b);

};