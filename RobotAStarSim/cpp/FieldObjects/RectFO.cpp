#include "../../include/FieldObjects/RectFO.h"
#include "../../include/Imports/PG_math.h"

RectFO::RectFO() {

}

RectFO::RectFO(float x, float y, float l, float w) : FieldObject(x, y) {
	length = l;
	width = w;
}

bool RectFO::touching_point(Translation2d point) {
	return (point.getX() >= c_x - 0.5 * length && point.getX() <= c_x + 0.5 * length &&
		point.getY() >= c_y - 0.5 * width && point.getY() <= c_y + 0.5 * width);
}

bool RectFO::touching_line(Translation2d l1, Translation2d l2) {
	Translation2d q1(c_x + 0.5 * length, c_y + 0.5 * width);
	Translation2d q2(c_x - 0.5 * length, c_y + 0.5 * width);
	Translation2d q3(c_x - 0.5 * length, c_y - 0.5 * width);
	Translation2d q4(c_x + 0.5 * length, c_y - 0.5 * width);

	return (PG_math::intersect_lineseg(l1, l2, q1, q2) || PG_math::intersect_lineseg(l1, l2, q2, q3) || PG_math::intersect_lineseg(l1, l2, q3, q4) || PG_math::intersect_lineseg(l1, l2, q4, q1));
}