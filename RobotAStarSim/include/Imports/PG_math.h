#pragma once

#include "Translation2d.h"

struct PG_math {
	static short clamp(short min, short val, short max);
	static float clamp(float min, float val, float max);

	static float dot_f(Translation2d t1, Translation2d t2);
	static float point_from_lineseg_f(Translation2d l1, Translation2d l2, Translation2d point);

	static bool intersect_lineseg(Translation2d l11, Translation2d l12, Translation2d l21, Translation2d l22);
private:
	PG_math() = delete;
};