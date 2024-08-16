enum MoveType {
	Linear, Quadratic,
};


struct ObjectMove : BasicEvent {
	Object* obj;
	float xrate, yrate, xacc, yacc, xpos, ypos;
	float xgoal, ygoal;

	float wrate, hrate, wacc, hacc, wpos, hpos;
	float wgoal, hgoal;

	ObjectMove() {}
	ObjectMove(Object* obj, float x, float y, float w, float h, float time, BasicEvent** pointer) : BasicEvent(pointer) {
		this->obj = obj;
		xpos = obj->x, ypos = obj->y, wpos = obj->w, hpos = obj->h;
		xrate = (x - obj->x) * 1.f / time;
		yrate = (y - obj->y) * 1.f / time;
		wrate = (w - obj->w) * 1.f / time;
		hrate = (h - obj->h) * 1.f / time;
		xacc = 0, yacc = 0, wacc = 0, hacc = 0;
		xgoal = x, ygoal = y, wgoal = w, hgoal = h;
	}
	~ObjectMove() {}


	void loop() {
		if (!eb.running) return;
		if (eb.interupt) { interrupt(); return; }

		xpos += xrate * dt + 0.5 * xacc * dt * dt;
		ypos += yrate * dt + 0.5 * yacc * dt * dt;
		wpos += wrate * dt + 0.5 * wacc * dt * dt;
		hpos += hrate * dt + 0.5 * hacc * dt * dt;

		xrate += xacc * dt, yrate += yacc * dt, wrate += wacc * dt, hrate += hacc * dt;
		
		obj->setPos(xpos , ypos);
		obj->setScale(wpos, hpos);

		bool reachedXGoal = (obj->x >= xgoal && xrate >= 0) || (obj->x <= xgoal && xrate <= 0);
		bool reachedYGoal = (obj->y >= ygoal && yrate >= 0) || (obj->y <= ygoal && yrate <= 0);
		bool reachedWGoal = (obj->w >= wgoal && wrate >= 0) || (obj->w <= wgoal && wrate <= 0);
		bool reachedHGoal = (obj->h >= hgoal && hrate >= 0) || (obj->h <= hgoal && hrate <= 0);

		if (reachedXGoal && reachedYGoal && reachedWGoal && reachedHGoal) {
			obj->setPos(xgoal, ygoal);
			obj->setScale(wgoal, hgoal);

			end();
		}
	}

};