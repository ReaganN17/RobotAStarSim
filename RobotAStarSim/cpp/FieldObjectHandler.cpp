#pragma once

#include "../include/FieldObjectHandler.h"


std::vector<FieldObject*>  FieldObjectHandler::fieldobjs;

void FieldObjectHandler::add(FieldObject* fobj)
{
	if (std::find(fieldobjs.begin(), fieldobjs.end(), fobj) != fieldobjs.end()) { return; }

	fieldobjs.push_back(fobj);
}

void FieldObjectHandler::remove(FieldObject* fobj)
{
	auto key = std::find(fieldobjs.begin(), fieldobjs.end(), fobj);
	if (key == fieldobjs.end()) { return; }

	fieldobjs.erase(key);
}

bool FieldObjectHandler::pointIsTouchingAny(double x, double y)
{
	for (auto fobj : fieldobjs) {
		if (fobj->isTouching(x, y)) return true;
	}

	return false;
}

bool FieldObjectHandler::lineIsTouchingAny(double x0, double y0, double x1, double y1)
{
	for (auto fobj : fieldobjs) {
		if (fobj->lineIsTouching(x0, y0, x1, y1)) return true;
	}

	return false;
}
