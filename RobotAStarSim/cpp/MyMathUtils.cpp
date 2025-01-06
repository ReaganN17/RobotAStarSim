#include <cmath>
#include "../include/MYMathUtils.h"

int MyMathUtils::clamp(int min, int val, int max)
{
    if (val < min) return min;
    if (val > max) return max;

    return val;
}
double MyMathUtils::clamp(double min, double val, double max) {
    if (val < min) return min;
    if (val > max) return max;

    return val;
}

double MyMathUtils::distance(double x0, double y0, double x1, double y1) {
    return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

double MyMathUtils::distance_point_line(double x0, double y0, double x1, double y1, double xp, double yp)
{
    return abs((x1 - x0) * yp - (y1 - y0) * xp - x1 * y0 + x0 * y1) / distance(x0, y0, x1, y1);
}

bool MyMathUtils::lines_intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    double uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    double uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

bool MyMathUtils::line_intersect_rect(double x0, double y0, double x1, double y1, double xr, double yr, double wr, double hr)
{
    return lines_intersect(x0, y0, x1, y1, xr - wr * 0.5, yr - hr * 0.5, xr + wr * 0.5, yr - hr * 0.5) ||
        lines_intersect(x0, y0, x1, y1, xr + wr * 0.5, yr - hr * 0.5, xr + wr * 0.5, yr + hr * 0.5) ||
        lines_intersect(x0, y0, x1, y1, xr + wr * 0.5, yr + hr * 0.5, xr - wr * 0.5, yr + hr * 0.5) ||
        lines_intersect(x0, y0, x1, y1, xr - wr * 0.5, yr + hr * 0.5, xr - wr * 0.5, yr - hr * 0.5);
}

bool MyMathUtils::line_intersect_circle(double x0, double y0, double x1, double y1, double x2, double y2, double r2) {
    double b = 2.f * ((x0 - x2) * (x1 - x0) + (y0 - y2) * (y1 - y0));
    double c = ((x0 - x2) * (x0 - x2) + (y0 - y2) * (y0 - y2) - r2 * r2);
    double a = ((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));

    double discriminate = b * b - 4.f * a * c;

    if (discriminate < 0) { return false; }
    double val1 = (-b + sqrt(discriminate)) / (2.f * a);
    double val2 = (-b - sqrt(discriminate)) / (2.f * a);
    return ((0 <= val1 && val1 <= 1) || (0 <= val2 && val2 <= 1));
}