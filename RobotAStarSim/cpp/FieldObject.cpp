#include "../include/FieldObject.h"
#include "../include/FieldObjectHandler.h"
#include "../include/MyMathUtils.h"

FieldObject::FieldObject() {

}
FieldObject::FieldObject(double x, double y) : x(x), y(y)
{
	FieldObjectHandler::add(this);
}

FieldObject::FieldObject(Translation2d translation) : FieldObject(translation.x, translation.y)
{

}

FieldObject::FieldObject(double x, double y, double r) : x(x), y(y), r(r)
{
	FieldObjectHandler::add(this);
}

FieldObject::FieldObject(Translation2d translation, double r) : FieldObject(translation.x, translation.y, r)
{

}

FieldObject::FieldObject(double x, double y, double w, double h) : x(x), y(y), w(w), h(h)
{
	FieldObjectHandler::add(this);
}

FieldObject::FieldObject(Translation2d translation, double w, double h) : FieldObject(translation.x, translation.y, w, h)
{
}

void FieldObject::enableObstacle()
{
	obstacle = true;
}

void FieldObject::disableObstacle()
{
	obstacle = false;
}

bool FieldObject::isTouching(double x, double y)
{
    if (!obstacle) return false;

    if (r != -1) {
        return (x - this->x) * (x - this->x) + (y - this->y) * (y - this->y) <= r * r;
    }
    else if (w != -1 && h != -1) {
        return (this->x - 0.5 * w <= x && x <= this->x + 0.5 * w && this->y - 0.5 * h <= y && y <= this->y + 0.5 * h);
    }
    else {
        return (this->x == x && this->y == y);
    }
}

bool FieldObject::lineIsTouching(double x0, double y0, double x1, double y1)
{
    if (!obstacle) return false;

    if (r != -1) {
        return MyMathUtils::line_intersect_circle(x0, y0, x1, y1, x, y, r);
    }
    else if (w != -1 && h != -1) {
        return MyMathUtils::line_intersect_rect(x0, y0, x1, y1, x, y, w, h);
    }
    else {
        return MyMathUtils::distance_point_line(x0, y0, x1, y1, x, y);
    }
}
