#pragma once


#include "AStar.h"



class PathGen {

	
	
public:

	Grid a_star;
	PathGen();
	PathGen(float length, float width, float sample);

	std::vector<Translation2d> gen_pivots(Translation2d start, Translation2d end);

	void update_grid_fo();
};