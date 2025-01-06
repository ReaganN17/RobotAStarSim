#include "TaffyEngine.h"

#include "AutoPathGen.h"

float field_width = 8.21055;
float field_length = 16.5417;

PathPointGen ppg;
Object map;
Object background;
Object start_display;
Object end_display;
Object instructions;

FieldObject rMpillar;
FieldObject rRpillar;
FieldObject rLpillar;
FieldObject bMpillar;
FieldObject bRpillar;
FieldObject bLpillar;

Translation2d start;
Translation2d end;

std::vector<Translation2d> points;

void renderPaths();

void init() {
	SpriteHandler::addSprite("field_map", new Sprite("resources/map.png"));
	SpriteHandler::addSprite("start_display", new TextSprite("START: NA, NA"));
	SpriteHandler::addSprite("end_display", new TextSprite("END: NA, NA"));
	SpriteHandler::addSprite("instructions", new TextSprite("LEFT CLICK: START POSITION \nRIGHT CLICK: END POSITION \nENTER: GENERATE \nESC: EXIT"));

	new (&background) Object(0, 0, 960, 540, 0xa0a0a0, Z_FARBACK);
	new (&map) Object(-155,90, 650, 360, "field_map", Z_FARBACK);

	new (&start_display) Object(325, 200, 300, 50, "start_display", Z_BACK);
	new (&end_display) Object(325, 100, 300, 50, "end_display", Z_BACK);
	new (&instructions) Object(-200, -175, 500, 150, "instructions", Z_BACK);
	
	new(&ppg) PathPointGen(field_length, field_length, 0.15);

	new (&rMpillar) FieldObject(3.30454, 4.51358, (0.5747258 + 0.4191));
	new (&rRpillar) FieldObject(5.5372, 5.8039, (0.5747258 + 0.4191));
	new (&rLpillar) FieldObject(5.5245, 3.23723, (0.5747258 + 0.4191));
	new (&bMpillar) FieldObject(12.99845, 4.5085, (0.5747258 + 0.4191));
	new (&bRpillar) FieldObject(10.78611, 5.79882, (0.5747258 + 0.4191));
	new (&bLpillar) FieldObject(10.7823, 3.2385, (0.5747258 + 0.4191));
}

void loop() {
	if (Input::is_pressed(VK_LBUTTON)) {
		new (&start) Translation2d(
			(Input::getMouse().x - map.getX() + map.getW() * 0.5) /  map.getW() * field_length,
			(Input::getMouse().y - map.getY() + map.getH() * 0.5) / map.getH() * field_width
		);

		renderPaths();

		SpriteHandler::updateSprite(
			"start_display", new TextSprite(
				"START: " + math::display_sig(start.x, 3) + (std::string)", " + math::display_sig(start.y, 3)));

	}
	
	if (Input::is_pressed(VK_RBUTTON)) {
		new (&end) Translation2d(
			(Input::getMouse().x - map.getX() + map.getW() * 0.5) / map.getW() * field_length,
			(Input::getMouse().y - map.getY() + map.getH() * 0.5) / map.getH() * field_width
		);
		
		renderPaths();

		SpriteHandler::updateSprite(
			"end_display", new TextSprite(
				"END: " + math::display_sig(end.x, 4) + (std::string)", " + math::display_sig(end.y, 4)));
	}
	
	if (Input::is_pressed(VK_RETURN)) {
		points = ppg.generateTrajectory(start, end);

		renderPaths();
	}
	
	if (Input::is_down(VK_ESCAPE)) { EngineBase::end(); }
}

int main() {

	EngineBase::run(init, loop);

	return 0;
}

//Renders start, end, path, and pivot positions in little squares

std::vector<Object*> renders;

void renderPaths() {
	for (Object* obj : renders) {
		if (obj != nullptr) { delete obj; }
		obj = nullptr;
	}

	renders.clear();

	renders.push_back(new Object(
		(start.x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
		(start.y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
		ppg.grid.gridxScale * map.getW() / field_length * 0.8,
		ppg.grid.gridyScale * map.getH() / field_width * 0.8,
		0xffffff, Z_FARFRONT)
	);

	renders.push_back(new Object(
		(end.x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
		(end.y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
		ppg.grid.gridxScale * map.getW() / field_length * 0.8,
		ppg.grid.gridyScale * map.getH() / field_width * 0.8,
		(u32)0x000000, Z_FARFRONT)
	);

	if (ppg.grid.endNode != nullptr) {
		renders.push_back(new Object(
			(ppg.nodeToT2d(ppg.grid.endNode).x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(ppg.nodeToT2d(ppg.grid.endNode).y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			ppg.grid.gridxScale * map.getW() / field_length * 0.8,
			ppg.grid.gridyScale * map.getH() / field_width * 0.8,
			0xff0000, Z_FRONT)
		);

	}
	if (ppg.grid.startNode != nullptr) {
		renders.push_back(new Object(
			(ppg.nodeToT2d(ppg.grid.startNode).x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(ppg.nodeToT2d(ppg.grid.startNode).y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			ppg.grid.gridxScale * map.getW() / field_length * 0.8,
			ppg.grid.gridyScale * map.getH() / field_width * 0.8,
			0x00ff00, Z_FRONT)
		);
	}

	if (ppg.grid.endNode != nullptr && ppg.grid.endNode->parent != nullptr) {
		Node* curNode = ppg.grid.endNode->parent;
		while (curNode != ppg.grid.startNode) {
			renders.push_back(new Object(
				(ppg.nodeToT2d(curNode).x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
				(ppg.nodeToT2d(curNode).y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
				ppg.grid.gridxScale * map.getW() / field_length * 0.8,
				ppg.grid.gridyScale * map.getH() / field_width * 0.8,
				0xff00ff, Z_MIDDLE)
			);

			curNode = curNode->parent;
		}
	}

	for (int i = 0; i < ppg.grid.gridLength * ppg.grid.gridWidth; i++) {
		if (ppg.grid.nodes[i].obstacle) {
			renders.push_back(new Object(
				(ppg.nodeToT2d(&ppg.grid.nodes[i]).x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
				(ppg.nodeToT2d(&ppg.grid.nodes[i]).y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
				ppg.grid.gridxScale * map.getW() / field_length * 0.8,
				ppg.grid.gridyScale * map.getH() / field_width * 0.8,
				0xa0a0a0, Z_MIDDLE)
			);
		}
	}
	for (Translation2d pivot : ppg.pivots) {
		renders.push_back(new Object(
			(pivot.x / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(pivot.y / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			ppg.grid.gridxScale * map.getW() / field_length * 0.8,
			ppg.grid.gridyScale * map.getH() / field_width * 0.8,
			0x000000ff, Z_FRONT)
		);

	}

}