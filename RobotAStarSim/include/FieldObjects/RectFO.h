#pragma once

#include "FieldObject.h"

class RectFO : public FieldObject {
	float length = 0, width = 0;

public:
	RectFO();

	RectFO(float x, float y, float l, float w);

	bool touching_point(Translation2d point);

	bool touching_line(Translation2d l1, Translation2d l2);
};