struct PathPoint {
	float x, y;
	float disFromStart;
	bool pivotPoint = false;

	PathPoint(float x, float y) : x(x), y(y) {}
	bool isEqual(PathPoint* guh) {
		return guh->x == x && guh->y == y;
	}

	friend bool operator==(const PathPoint& n1, const PathPoint& n2);
};

bool operator==(const PathPoint& n1, const PathPoint& n2) {
	return n1.x == n2.x && n1.y == n2.y && n1.pivotPoint == n2.pivotPoint;
}


struct RobotPath : Object {
	
	RobotPath() {}

	vector<PathPoint> gridPoints;
	vector<PathPoint> pivotPoints;
	vector<PathPoint> pathPoints;

	Obstacles* obs = nullptr;
	MapGrid* ms = nullptr;

	RobotPath(MapGrid* ms) : ms(ms), obs(ms->obs), Object(ms->x, ms->y, ms->w, ms->h, 4) {}

	void createRobotPath() {
		Node* curNode = ms->nodeEnd;

		gridPoints.clear();

		while (curNode->parent != nullptr) {
			gridPoints.push_back(PathPoint(curNode->x, curNode->y));

			curNode = curNode->parent;
		}
		gridPoints.push_back(PathPoint(curNode->x, curNode->y));

		reverse(gridPoints.begin(), gridPoints.end());
	}

	float distancePath(PathPoint* p1, PathPoint* p2) {
		return heuristicRaw(p1->x, p1->y, p2->x, p2->y);
	}

	void genPivots() {
		pivotPoints.clear();

		gridPoints.front().pivotPoint = true;
		gridPoints.back().pivotPoint = true;

		pivotPoints.push_back(PathPoint(gridPoints.front()));
		pivotPoints.push_back(PathPoint(gridPoints.back()));

		int curIndex = 0;

		while (curIndex < pivotPoints.size() - 1) {

			
			if (lineTouching(&pivotPoints.at(curIndex), &pivotPoints.at(curIndex + 1))) {
				float maxR = -1;
				PathPoint maxPoint = pivotPoints.at(curIndex);
				int pivotCounter = 0;

				for (auto i : gridPoints) {

					

					if (curIndex == pivotCounter - 1) {
						if (i.pivotPoint) break;

						float res = pointFromLinePP(&pivotPoints.at(curIndex), &pivotPoints.at(curIndex + 1), &i);

						if (maxR < res) {
							maxR = res;
							maxPoint = i;
						}
						
					}

					if (i.pivotPoint) {
						pivotCounter++;
					}
					
					
				}

				if (!maxPoint.isEqual(&pivotPoints.at(curIndex)) && !maxPoint.isEqual(&pivotPoints.at(curIndex + 1))) {
					find(gridPoints.begin(), gridPoints.end(), maxPoint)->pivotPoint = true;
					maxPoint.pivotPoint = true;

					vector<PathPoint>::iterator it = pivotPoints.begin();
					advance(it, curIndex + 1);
					pivotPoints.insert(it, maxPoint);
				}
				else {

					
					curIndex++;
				}
				
				
				
			}
			else {

				curIndex++;
			}


			
			
			

			
		}

	}

	bool lineTouching(PathPoint* pp1, PathPoint* pp2) {
		if (obs == nullptr) return false;

		return obs->lineTouchingAny(pp1->x, pp1->y, pp2->x, pp2->y);
	}

	float pointFromLinePP(PathPoint* pp1, PathPoint* pp2, PathPoint* point) {
		return pointFromLine(pp1->x, pp1->y, pp2->x, pp2->y, point->x, point->y);
	}


	void render() {
		for (auto i : pivotPoints) {
			ms->drawGridNode(i.x, i.y, 1, 0x0000ff);
		}
		
		
		for (auto i : pathPoints) {
			ms->drawGridNode(i.x, i.y, 0.5, 0xff00ff);
		}
		
	}

	float interpolation(float val0, float val1, float val2, float val3, float t) {
		t -= (int)t;
		
		val0 = clampF(val1 - 5, val0, val1 + 5);
		val3 = clampF(val2 - 5, val3, val2 + 5);

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.f * tt - t;
		float q2 = 3.f * ttt - 5.f * tt + 2.f;
		float q3 = -3.f * ttt + 4.f * tt + t;
		float q4 = ttt - tt;

		return (0.5f * (val0 * q1 + val1 * q2 + val2 * q3 + val3 * q4));
	}

	PathPoint interpolatePath(vector<PathPoint>* path, int index, float t) {
		
		float x = interpolation(path->at(max(0, index - 1)).x, path->at(index).x,
				path->at(min(path->size() - 1, index + 1)).x, path->at(min(path->size() - 1, index + 2)).x, t);

		float y = interpolation(path->at(max(0, index - 1)).y, path->at(index).y,
				path->at(min(path->size() - 1, index + 1)).y, path->at(min(path->size() - 1, index + 2)).y, t);
		

		return PathPoint(x, y);
	}


	void genPath() {
		pathPoints.clear();

		for (int i = 0; i < pivotPoints.size() - 1; i++) {
			float dis = distancePath(&pivotPoints.at(i), &pivotPoints.at(i + 1));
			int divisor = 2 * dis;


			for (float t = 0; t < 1; t += 1.f / divisor) {
				pathPoints.push_back(interpolatePath(&pivotPoints, i, t));
				if (t == 0) pathPoints.back().pivotPoint = true;
			}
		}

		pathPoints.push_back(pivotPoints.back());
	}
	
};