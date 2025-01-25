#pragma once

#include "FieldObject.h"
#include "../Imports/Translation2d.h"

class CircleFO : public FieldObject {
	float radius = 0;

public: 
	CircleFO();

	CircleFO(float x, float y, float radius);

	bool touching_line(Translation2d l1, Translation2d l2);

	bool touching_point(Translation2d point);
};