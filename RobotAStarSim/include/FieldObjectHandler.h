#pragma once

#include <vector>
#include "FieldObject.h"

class FieldObjectHandler final {

    friend class FieldObject;
    friend struct PathPointGen;
    friend struct Grid;

    FieldObjectHandler() = delete;

    static std::vector<FieldObject*> fieldobjs;

    static void add(FieldObject* fobj);

    static void remove(FieldObject* fobj);

    static bool pointIsTouchingAny(double x, double y);

    static bool lineIsTouchingAny(double x0, double y0, double x1, double y1);
};