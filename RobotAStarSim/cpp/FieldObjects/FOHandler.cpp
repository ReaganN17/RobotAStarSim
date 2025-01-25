#include "../../include/FieldObjects/FOHandler.h"

std::vector<FieldObject*> FOHandler::fobjs;

void FOHandler::addFO(FieldObject* fo) {
	removeFO(fo);

	fobjs.push_back(fo);
}

void FOHandler::removeFO(FieldObject* fo) {
	auto key = std::find(fobjs.begin(), fobjs.end(), fo);

	if (key != fobjs.end()) { fobjs.erase(key); }
}

bool FOHandler::any_touching_line(Translation2d l1, Translation2d l2) {
	for (auto fo : fobjs) {
		if (fo->touching_line(l1, l2)) return true;
	}
	return false;
}

bool FOHandler::any_touching_point(Translation2d point) {
	for (auto fo : fobjs) {
		if (fo->touching_point(point)) return true;
	}
	return false;
}