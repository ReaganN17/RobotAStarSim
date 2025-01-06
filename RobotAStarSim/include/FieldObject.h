#pragma once

#include "mock WPI/Translation2d.h"

class FieldObject {
    double x, y, w = -1, h = -1, r = -1;
    bool obstacle = true;

    friend class FieldObjectHandler;

public:
    FieldObject();

    FieldObject(double x, double y);

    FieldObject(Translation2d translation);

    FieldObject(double x, double y, double r);

    FieldObject(Translation2d translation, double r);

    FieldObject(double x, double y, double w, double h);

    FieldObject(Translation2d translation, double w, double h);

    void enableObstacle();

    void disableObstacle();

private:
    bool isTouching(double x, double y);

    bool lineIsTouching(double x0, double y0, double x1, double y1);
};