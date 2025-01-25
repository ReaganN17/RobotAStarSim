#include "../../include/Imports/Translation2d.h"
#include <cmath>

Translation2d::Translation2d() {

}

Translation2d::Translation2d(double x, double y) :x(x), y(y){
}

double Translation2d::getDistance(Translation2d other) {
	return sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
}

double Translation2d::getX() {
	return x;
}

double Translation2d::getY() {
	return y;
}

Translation2d Translation2d::plus(Translation2d other) {
	return Translation2d(x + other.x, y + other.y);
}

Translation2d Translation2d::minus(Translation2d other) {
	return Translation2d(x - other.x, y - other.y);
}

Translation2d Translation2d::times(double scalar) {
	return Translation2d(x * scalar, y * scalar);
}