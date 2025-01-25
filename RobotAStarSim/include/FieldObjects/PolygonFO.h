#pragma once

#include "../Imports/Translation2d.h"
#include "FieldObject.h"
#include <vector>

class PolygonFO : public FieldObject {
	
	std::vector<Translation2d> vertices;

public:
	PolygonFO();

	PolygonFO(std::initializer_list<Translation2d> verticies);

	bool touching_line(Translation2d l1, Translation2d l2);

	bool touching_point(Translation2d point);
};