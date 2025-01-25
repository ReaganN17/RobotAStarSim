#pragma once


class Translation2d {

	double x = 0, y = 0;

public:
	Translation2d();

	Translation2d(double x, double y);

	double getDistance(Translation2d other);

	double getX();

	double getY();

	Translation2d plus(Translation2d other);

	Translation2d minus(Translation2d other);

	Translation2d times(double scalar);

};