#pragma once

struct MyMathUtils final {
	static int clamp(int min, int val, int max);
	static double clamp(double min, double val, double max);

	static double distance(double x0, double y0, double x1, double y1);
	static double distance_point_line(double x0, double y0, double x1, double y1, double xp, double yp);

	static bool lines_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
	static bool line_intersect_rect(double x0, double y0, double x1, double y1, double xr, double yr, double wr, double hr);

	static bool line_intersect_circle(double x0, double y0, double x1, double y1, double x2, double y2, double r2);

private:
	MyMathUtils() = delete;
};