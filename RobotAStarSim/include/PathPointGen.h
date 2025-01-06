#pragma once
#include <vector>
#include "FieldObjectHandler.h"
#include "mock WPI/Translation2d.h"
#include "Astar.h"
#include "TaffyEngine.h"

struct PathPointGen {
	
	std::vector<Translation2d> pivots;

	Grid grid;

	PathPointGen();
	PathPointGen(double length, double width, double sample_size);

	std::vector<Translation2d> generateTrajectory(Translation2d start, Translation2d end);

	bool generatePivots(Translation2d start, Translation2d end);

	Translation2d nodeToT2d(Node* node);

	Node* t2dToNode(Translation2d pose);

	bool lineIsTouchingAny(Node* n1, Node* n2);

	double pointFromLine(Node* n1, Node* n2, Node* point);
};