#include "../../include/FieldObjects/CircleFO.h"
#include "../../include/Imports/PG_math.h"

CircleFO::CircleFO() {

}

CircleFO::CircleFO(float x, float y, float radius) : FieldObject(x, y){
	this->radius = radius;
}

bool CircleFO::touching_line(Translation2d l1, Translation2d l2) {
	return PG_math::point_from_lineseg_f(l1, l2, Translation2d(c_x, c_y)) <= radius * radius;
}

bool CircleFO::touching_point(Translation2d point) {
	return (c_x - point.getX()) * (c_x - point.getX()) + (c_y - point.getY()) * (c_y - point.getY()) <= radius * radius;
}

