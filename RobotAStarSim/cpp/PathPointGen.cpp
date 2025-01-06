#include "../include/PathPointGen.h"
#include "../include/MyMathUtils.h"
#include "TaffyEngine.h"

PathPointGen::PathPointGen()
{

}

PathPointGen::PathPointGen(double length, double width, double sample_size)
{
	new (&grid) Grid(length, width, sample_size);

}

std::vector<Translation2d> PathPointGen::generateTrajectory(Translation2d start, Translation2d end)
{
	generatePivots(start, end);

	return pivots;
}

bool PathPointGen::generatePivots(Translation2d start, Translation2d end)
{
	grid.updateGrid();	
	pivots.clear();

	std::vector<Node*> backwardPivots;

	if (!grid.pathFind(t2dToNode(start), t2dToNode(end))) return false;

	backwardPivots.push_back(t2dToNode(end));
	backwardPivots.push_back(t2dToNode(start));

	Node* curNode = nullptr;
	Node* pivotNode = nullptr;
	u32 curIndex = 0;

	while (curIndex < backwardPivots.size() - 1) {
	
		curNode = backwardPivots.at(curIndex)->parent;

		if (lineIsTouchingAny(backwardPivots.at(curIndex), backwardPivots.at(curIndex + 1))) {

			double maxD = -1;

			while (curNode != backwardPivots.at(curIndex + 1)) {
				double d = pointFromLine(backwardPivots.at(curIndex), backwardPivots.at(curIndex + 1), curNode);

				if (maxD < d) {
					maxD = d;
					pivotNode = curNode;
				}

				curNode = curNode->parent;

			}

			if (pivotNode != nullptr) {
				backwardPivots.insert(backwardPivots.begin() + curIndex + 1, pivotNode);
			}
			else {
				curIndex++;
			}
			pivotNode = nullptr;
		}
		else {
			curIndex++;
		}
	}

	std::reverse(backwardPivots.begin(), backwardPivots.end());

	for (Node* n : backwardPivots) {
		pivots.push_back(nodeToT2d(n));
	}

	if (pivots.size() < 2) return false;
	pivots.erase(pivots.begin());
	pivots.erase(pivots.end() - 1);

	if (pivots.size() == 0) {
		pivots.push_back(Translation2d(0.5 * (start.x + end.x), 0.5 * (start.y + end.y)));
	}

	return true;

}

Translation2d PathPointGen::nodeToT2d(Node* node)
{
	return Translation2d(grid.gridxScale * (node->x - 0.5), grid.gridyScale * (node->y - 0.5));
}

Node* PathPointGen::t2dToNode(Translation2d pose)
{
	return grid.getNode((int)(pose.x / grid.gridxScale + 1), (int)(pose.y / grid.gridyScale + 1));
}

bool PathPointGen::lineIsTouchingAny(Node* n1, Node* n2)
{
	return FieldObjectHandler::lineIsTouchingAny(
		nodeToT2d(n1).x, nodeToT2d(n1).y,
		nodeToT2d(n2).x, nodeToT2d(n2).y
	);
}

double PathPointGen::pointFromLine(Node* n1, Node* n2, Node* point)
{
	return MyMathUtils::distance_point_line(
		nodeToT2d(n1).x, nodeToT2d(n1).y,
		nodeToT2d(n2).x, nodeToT2d(n2).y,
		nodeToT2d(point).x, nodeToT2d(point).y
	);
}


