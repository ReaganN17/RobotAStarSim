inline int clamp(int min, int val, int max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

inline float clampF(float min, float val, float max) {
	if (val > max) return max;
	if (val < min) return min;
	return val;
}

bool linesIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
}

bool lineIntersectCircle(float x0, float y0, float x1, float y1, float x2, float y2, float r2) {
	float b = 2.f * ((x0 - x2) * (x1 - x0) + (y0 - y2) * (y1 - y0));
	float c = ((x0 - x2) * (x0 - x2) + (y0 - y2) * (y0 - y2) - r2 * r2);
	float a = ((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));

	float discriminate = b * b - 4.f * a * c;

	if (discriminate < 0) { return false; }
	float val1 = (-b + sqrtf(discriminate)) / (2.f * a);
	float val2 = (-b - sqrtf(discriminate)) / (2.f * a);
	return ((0 <= val1 && val1 <= 1) || (0 <= val2 && val2 <= 1));
}

float heuristicRaw(float x0, float y0, float x1, float y1) {
	return sqrtf((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

float pointFromLine(float x0, float y0, float x1, float y1, float xp, float yp) {
	return abs((x1 - x0) * yp - (y1 - y0) * xp - x1 * y0 + x0 * y1) / heuristicRaw(x0, y0, x1, y1);
}


