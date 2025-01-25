#pragma once

#include <vector>
#include "FieldObject.h"
#include "../Imports/Translation2d.h"


class FOHandler {

	FOHandler() = delete;

	static std::vector<FieldObject*> fobjs;

public:
	static void addFO(FieldObject* fo);

	static void removeFO(FieldObject* fo);

	static bool any_touching_line(Translation2d l1, Translation2d l2);

	static bool any_touching_point(Translation2d point);
};