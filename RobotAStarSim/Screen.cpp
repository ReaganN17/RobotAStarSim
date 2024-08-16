#include "MapGrid.cpp"

struct MapScreen : BasicEvent {

	MapScreen() {}
	~MapScreen() {}

	MapScreen(BasicEvent** pointer) : BasicEvent(pointer) {}

	Object erm;
	MapGrid guh;
	Obstacles obs;
	RobotPath rp;

	float x = 0;

	void init() {
		new (&erm) Object(0, 0, 650, 360, "resources/map.png", 2);
		new (&obs) Obstacles();
		//sqrt 20.48 - half of robot width
		//3.3 pillar radius
		//0.5 - 2.5 inch butter
		obs.addCircle(26.02, 35.54, sqrtf(20.48) + 3.3 + 0.5);
		obs.addCircle(43.5, 25.47, sqrtf(20.48) + 3.3 + 0.5);
		obs.addCircle(43.6, 45.7, sqrtf(20.48) + 3.3 + 0.5);
		obs.addCircle(84.93, 45.66, sqrtf(20.48) + 3.3 + 0.5);
		obs.addCircle(84.9, 25.5, sqrtf(20.48) + 3.3 + 0.5);
		obs.addCircle(102.35, 35.5, sqrtf(20.48) + 3.3 + 0.5);

		new (&guh) MapGrid(&erm, 130, 72, &obs);
		new (&rp) RobotPath(&guh);

		guh.setStart(2, 9);
		guh.setEnd(30, 30);
	}

	void loop() {
		debug_num = obs.lineTouchingAny(20, 19, 24, 19);
		if (pressed(W)) { rp.createRobotPath(); rp.genPivots(); }
		if (pressed(A)) { rp.genPath(); }
	}


};