#include "AutoPath.cpp"

struct MapScreen : BasicEvent {

	MapScreen() {}
	~MapScreen() {}

	MapScreen(BasicEvent** pointer) : BasicEvent(pointer) {}

	FieldObject rMpillar;
	FieldObject rRpillar;
	FieldObject rLpillar;
	FieldObject bMpillar;
	FieldObject bRpillar;
	FieldObject bLpillar;

	FieldObjects fobjs;
	FieldGrid fg;
	AutoPath path;



	void init() {

		new(&fobjs) FieldObjects();

		new (&rMpillar) FieldObject(130.1, 177.7, (22.627 + 16.5 + 2.5)); rMpillar.obstacle = true; fobjs.add(&rMpillar);
		new (&rRpillar) FieldObject(218, 228.5, (22.627 + 16.5 + 2.5)); rRpillar.obstacle = true; fobjs.add(&rRpillar);
		new (&rLpillar) FieldObject(217.5, 127.45, (22.627 + 16.5 + 2.5)); rLpillar.obstacle = true; fobjs.add(&rLpillar);
		new (&bMpillar) FieldObject(511.75, 177.5, (22.627 + 16.5 + 2.5)); bMpillar.obstacle = true; fobjs.add(&bMpillar);
		new (&bRpillar) FieldObject(424.65, 228.3, (22.627 + 16.5 + 2.5)); bRpillar.obstacle = true; fobjs.add(&bRpillar);
		new (&bLpillar) FieldObject(424.5, 127.5, (22.627 + 16.5 + 2.5)); bLpillar.obstacle = true; fobjs.add(&bLpillar);
		
		new(&fg) FieldGrid("resources/map.png", 0, 0, 650, 360, 1, 5, &fobjs);
		new(&path) AutoPath(&fg);
	}

	void loop() {
		if (isdown(SHIFT) && pressed(LEFT_MOUSE)) { fg.setStart(((mouse.x - fg.dx) / fg.scale / fg.gxscale) - 0.5, ((mouse.y - fg.dy) / fg.scale / fg.gyscale) - 0.5); }
		if (isdown(CTRL) && pressed(LEFT_MOUSE)) { fg.setEnd(((mouse.x - fg.dx) / fg.scale / fg.gxscale) - 0.5, ((mouse.y - fg.dy) / fg.scale / fg.gyscale) - 0.5); }

		if (pressed(KEY_1)) { fg.pathFind(); path.createGridPath(); }
		if (pressed(KEY_2)) { path.createPivotPath(); }
		if (pressed(KEY_3)) { path.createAutoPath(); }

	}


};