#include "../include/Astar.h"
#include <algorithm>
#include "../include/FieldObjectHandler.h"
#include <list>
#include "../include/MyMathUtils.h"

Node::Node() {
	x = 0, y = 0;
	resetGoals();
}

Node::Node(int x, int y) : x(x), y(y)
{
	resetGoals();
}

void Node::addNeighbour(Node* other)
{
	if (std::find(neighbours.begin(), neighbours.end(), other) == neighbours.end() && other != this) {
		neighbours.push_back(other);
	}
}

void Node::resetGoals()
{
	parent = nullptr;
	visited = false;
	fGlobalGoal = INFINITY;
	fLocalGoal = INFINITY;
}

bool Node::neighboursObstacle()
{
	for (Node* node : neighbours) {
		if (node->obstacle) return true;
	}
	return false;
}

Grid::Grid() {}

Grid::Grid(double length, double width, double sample_size)
{
	gridxScale = std::min(sample_size, length);
	gridyScale = std::min(sample_size, width);

	gridLength = (int)(length / gridxScale);
	gridWidth = (int)(width / gridyScale);

	gridxScale = length / gridLength;
	gridyScale = width / gridWidth;

	createGrid(gridLength, gridWidth);
}

Grid::~Grid() {
	if (nodes != nullptr)delete[] nodes;
	nodes = nullptr;
}

void Grid::updateGrid()
{
	for (int x = 0; x < gridLength; x++) {
		for (int y = 0; y < gridWidth; y++) {
			nodes[y * gridLength + x].obstacle = (
				FieldObjectHandler::pointIsTouchingAny((x + 0.5) * gridxScale, (y + 0.5) * gridyScale));
		}
	}
}

bool Grid::pathFind(Node* start, Node* end)
{
	for (int i = 0; i < gridWidth * gridLength; i++) {
		nodes[i].resetGoals();
	}

	startNode = start;
	endNode = end;

	if (startNode == nullptr || endNode == nullptr) return false;

	Node* curNode = startNode;
	startNode->fLocalGoal = 0.0f;
	startNode->fGlobalGoal = heuristic(startNode, endNode);

	std::list<Node*> listNotTestedNodes;
	listNotTestedNodes.push_back(startNode);

	while (!listNotTestedNodes.empty() && curNode != endNode)
	{
		listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		curNode = listNotTestedNodes.front();
		curNode->visited = true;

		for (auto nodeNeighbour : curNode->neighbours)
		{
			if (!nodeNeighbour->visited && !nodeNeighbour->obstacle && !nodeNeighbour->neighboursObstacle() )
				listNotTestedNodes.push_back(nodeNeighbour);

			double fPossiblyLowerGoal = curNode->fLocalGoal + heuristic(curNode, nodeNeighbour);

			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = curNode;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, endNode);
			}
		}
	}

	return endNode->parent != nullptr;
}

Node* Grid::getNode(int x, int y)
{
	x = MyMathUtils::clamp(0, x, gridLength - 1);
	y = MyMathUtils::clamp(0, y, gridWidth - 1);
	return &nodes[x + y * gridLength];
}

void Grid::createGrid(int l, int w)
{
	gridWidth = w;
	gridLength = l;
	nodes = new Node[w * l];

	for (int x = 0; x < l; x++) {
		for (int y = 0; y < w; y++) {
			nodes[y * l + x].x = x;
			nodes[y * l + x].y = y;
		}
	}

	for (int x = 0; x < l; x++) {
		for (int y = 0; y < w; y++) {
			for (int nx = -1; nx < 2; nx++) {
				for (int ny = -1; ny < 2; ny++) {
					nodes[y * l + x].addNeighbour(getNode(x + nx, y + ny));
				}
			}
		}
	}
}

double Grid::heuristic(Node* a, Node* b)
{
	return MyMathUtils::distance(a->x, a->y, b->x, b->y);
}
