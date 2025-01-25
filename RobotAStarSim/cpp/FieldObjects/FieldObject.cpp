#include "../../include/FieldObjects/FieldObject.h"
#include "../../include/Imports/PG_math.h"


FieldObject::FieldObject() {}

FieldObject::FieldObject(float x, float y): c_x(x), c_y(y){}

bool FieldObject::touching_line(Translation2d l1, Translation2d l2) {
	return PG_math::point_from_lineseg_f(l1, l2, Translation2d(c_x, c_y)) == 0;
}

bool FieldObject::touching_point(Translation2d point) {
	return c_x == point.getX() && c_y == point.getY();
}