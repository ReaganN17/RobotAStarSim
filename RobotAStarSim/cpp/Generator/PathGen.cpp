#include "../../include/Generator/PathGen.h"
#include "../../include/FieldObjects/FOHandler.h"
#include "../../include/Imports/PG_math.h"

PathGen::PathGen() {
	 
}

PathGen::PathGen(float length, float width, float sample_size) {

	new (&a_star) Grid(length, width, sample_size);
}

std::vector<Translation2d> PathGen::gen_pivots(Translation2d start, Translation2d end) {
	std::vector<Translation2d> pivots;

	std::vector<Node*> backward_nodes;

	if (!a_star.pathFind(a_star.t2d_to_node(start), a_star.t2d_to_node(end))) return pivots;

	backward_nodes.push_back(a_star.t2d_to_node(end));
	backward_nodes.push_back(a_star.t2d_to_node(start));

	Node* curNode = nullptr;
	Node* pivotNode = nullptr;
	short curIndex = 0;

	while (curIndex < backward_nodes.size() - 1) {

		curNode = backward_nodes.at(curIndex)->parent;

		if (FOHandler::any_touching_line(a_star.node_to_t2d(backward_nodes.at(curIndex)), a_star.node_to_t2d(backward_nodes.at(curIndex + 1)))) {
			
			float maxD = -1;

			while (curNode != backward_nodes.at(curIndex + 1)) {
				float d = PG_math::point_from_lineseg_f(
					a_star.node_to_t2d(backward_nodes.at(curIndex)), 
					a_star.node_to_t2d(backward_nodes.at(curIndex + 1)), 
					a_star.node_to_t2d(curNode));

				if (maxD < d) {
					maxD = d;
					pivotNode = curNode;
				}

				curNode = curNode->parent;

			}

			if (pivotNode != nullptr) {
				backward_nodes.insert(backward_nodes.begin() + curIndex + 1, pivotNode);
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

	std::reverse(backward_nodes.begin(), backward_nodes.end());

	for (Node* n : backward_nodes) {
		pivots.push_back(a_star.node_to_t2d(n));
	}

	
	if (pivots.size() < 2) return pivots;
	pivots.erase(pivots.begin());
	pivots.erase(pivots.end() - 1);

	if (pivots.size() == 0) {
		pivots.push_back(end.plus(start.minus(end).times(0.5)));
	}
	

	return pivots;
}


void PathGen::update_grid_fo() {
	a_star.update_fo();
}