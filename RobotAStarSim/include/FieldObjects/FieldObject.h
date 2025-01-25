#pragma once

#include "../Imports/Translation2d.h"

class FieldObject {

protected:
	float c_x = 0, c_y = 0;

public:
	FieldObject();

	FieldObject(float x, float y);

	virtual bool touching_line(Translation2d l1, Translation2d l2);

	virtual bool touching_point(Translation2d point);
};