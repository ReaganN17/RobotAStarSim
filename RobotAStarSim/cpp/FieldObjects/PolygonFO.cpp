#include "../../include/FieldObjects/PolygonFO.h"
#include "../../include/Imports/PG_math.h"

PolygonFO::PolygonFO() {}

PolygonFO::PolygonFO(std::initializer_list<Translation2d> vertices) : vertices(vertices) {

}

bool PolygonFO::touching_line(Translation2d l1, Translation2d l2) {
	for (int i = 0; i < vertices.size(); i++) {
		if (PG_math::intersect_lineseg(vertices.at(i), vertices.at((i + 1)%vertices.size()), l1, l2)) return true;
	}
	return false;
}

bool PolygonFO::touching_point(Translation2d point) {
	for (int i = 1; i < vertices.size(); i++) {
		if (PG_math::point_from_lineseg_f(vertices.at(i - 1), vertices.at(i), point) == 0) return true;
	}
	return false;
}