#include "../../include/Generator/AStar.h"
#include "../../include/FieldObjects/RectFO.h"
#include "../../include/FieldObjects/FOHandler.h"
#include "../../include/Imports/PG_math.h"
#include <list>

Node::Node() {

}

Node::Node(short x, short y) :x(x), y(y) {

}

void Node::addNeighbour(Node* other) {
	auto key = std::find(neighbours.begin(), neighbours.end(), other);

	if (key == neighbours.end() && other != this) neighbours.push_back(other);
}

void Node::resetGoals() {
	parent = nullptr;
	visited = false;
	fGlobalGoal = FLT_MAX;
	fLocalGoal = FLT_MAX;
}


Grid::Grid() {

}

Grid::Grid(float length, float width, float sample_size) {
	x_scale = std::min(sample_size, length);
	y_scale = std::min(sample_size, width);

	this->length = (short)(length / x_scale);
	this->width = (short)(width / y_scale);

	x_scale = length / this->length;
	y_scale = width / this->width;

	createGrid(this ->length, this ->width);
}


void Grid::update_fo() {
	for (int x = 0; x < length; x++) {
		for (int y = 0; y < width; y++) {
			Translation2d pos = node_to_t2d(&nodes[y * length + x]);
			Translation2d q1(pos.getX() + 0.5 * x_scale, pos.getY() + 0.5 * y_scale);
			Translation2d q2(pos.getX() - 0.5 * x_scale, pos.getY() + 0.5 * y_scale);
			Translation2d q3(pos.getX() - 0.5 * x_scale, pos.getY() - 0.5 * y_scale);
			Translation2d q4(pos.getX() + 0.5 * x_scale, pos.getY() - 0.5 * y_scale);

			nodes[y * length + x].obstacle = (FOHandler::any_touching_line(q1, q2) || FOHandler::any_touching_line(q2, q3) || FOHandler::any_touching_line(q3, q4) || FOHandler::any_touching_line(q4, q1));
		}
	}
}

bool Grid::pathFind(Node* start, Node* end) {
	for (int i = 0; i < width * length; i++) {
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
			if (!nodeNeighbour->visited && !nodeNeighbour->obstacle)
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

Node* Grid::getNode(short x, short y) {
	x = PG_math::clamp(0, x, length - 1);
	y = PG_math::clamp(0, y, width - 1);

	return &nodes[y * length + x];
}

void Grid::createGrid(short l, short w) {
	width = w;
	length = l;
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
float Grid::heuristic(Node* a, Node* b) {
	return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

Translation2d Grid::node_to_t2d(Node* node) {
	return Translation2d(x_scale * (node->x - 0.5), y_scale * (node->y - 0.5));
}
Node* Grid::t2d_to_node(Translation2d pose) {
	return getNode((short)(pose.getX() / x_scale + 1), (short)(pose.getY() / y_scale + 1));
}