#include "TaffyEngine.h"

#include "AutoPathGen.h"

//meters
double field_length = 690.875 * 0.0254;
double field_width = 317 * 0.0254;
double sample_size = 0.25;


Object map;
Object background;
Object start_display;
Object end_display;
Object instructions;

PathGen generator;
PolygonFO polygon;

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
	
	new (&generator) PathGen(field_length, field_width, sample_size);

	new (&polygon) PolygonFO({ Translation2d(0.89, 3.84), Translation2d(1.8, 0.6), Translation2d(4.57, 2.69) });

	FOHandler::addFO(&polygon);

	generator.update_grid_fo();
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
				"START: " + math::display_sig(start.getX(), 3) + (std::string)", " + math::display_sig(start.getY(), 3)));

	}
	
	if (Input::is_pressed(VK_RBUTTON)) {
		new (&end) Translation2d(
			(Input::getMouse().x - map.getX() + map.getW() * 0.5) / map.getW() * field_length,
			(Input::getMouse().y - map.getY() + map.getH() * 0.5) / map.getH() * field_width
		);
		
		renderPaths();

		SpriteHandler::updateSprite(
			"end_display", new TextSprite(
				"END: " + math::display_sig(end.getX(), 4) + (std::string)", " + math::display_sig(end.getY(), 4)));
	}
	
	if (Input::is_pressed(VK_RETURN)) {
		points = generator.gen_pivots(start, end);

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
		(start.getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
		(start.getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
		generator.a_star.x_scale * map.getW() / field_length * 0.8,
		generator.a_star.y_scale * map.getH() / field_width * 0.8,
		0xffffff, Z_FARFRONT)
	);

	renders.push_back(new Object(
		(end.getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
		(end.getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
		generator.a_star.x_scale * map.getW() / field_length * 0.8,
		generator.a_star.y_scale * map.getH() / field_width * 0.8,
		(u32)0x000000, Z_FARFRONT)
	);

	if (generator.a_star.endNode != nullptr) {
		renders.push_back(new Object(
			(generator.a_star.node_to_t2d(generator.a_star.endNode).getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(generator.a_star.node_to_t2d(generator.a_star.endNode).getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			generator.a_star.x_scale * map.getW() / field_length * 0.8,
			generator.a_star.y_scale * map.getH() / field_width * 0.8,
			0xff0000, Z_FRONT)
		);

	}
	if (generator.a_star.startNode != nullptr) {
		renders.push_back(new Object(
			(generator.a_star.node_to_t2d(generator.a_star.startNode).getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(generator.a_star.node_to_t2d(generator.a_star.startNode).getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			generator.a_star.x_scale * map.getW() / field_length * 0.8,
			generator.a_star.y_scale * map.getH() / field_width * 0.8,
			0x00ff00, Z_FRONT)
		);
	}

	if (generator.a_star.endNode != nullptr && generator.a_star.endNode->parent != nullptr) {
		Node* curNode = generator.a_star.endNode->parent;
		while (curNode != generator.a_star.startNode) {
			renders.push_back(new Object(
				(generator.a_star.node_to_t2d(curNode).getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
				(generator.a_star.node_to_t2d(curNode).getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
				generator.a_star.x_scale * map.getW() / field_length * 0.8,
				generator.a_star.y_scale * map.getH() / field_width * 0.8,
				0xff00ff, Z_FRONT)
			);

			curNode = curNode->parent;
		}
	}

	for (int i = 0; i < generator.a_star.length * generator.a_star.width; i++) {
		if (generator.a_star.nodes[i].obstacle) {
			renders.push_back(new Object(
				(generator.a_star.node_to_t2d(&generator.a_star.nodes[i]).getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
				(generator.a_star.node_to_t2d(&generator.a_star.nodes[i]).getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
				generator.a_star.x_scale * map.getW() / field_length * 0.8,
				generator.a_star.y_scale * map.getH() / field_width * 0.8,
				0xa0a0a0, Z_FRONT)
			);
		}
	}
	for (Translation2d pivot : points) {
		renders.push_back(new Object(
			(pivot.getX() / field_length * map.getW()) + map.getX() - map.getW() * 0.5,
			(pivot.getY() / field_width * map.getH()) + map.getY() - map.getH() * 0.5,
			generator.a_star.x_scale * map.getW() / field_length * 0.8,
			generator.a_star.y_scale * map.getH() / field_width * 0.8,
			0x000000ff, Z_FRONT)
		);

	}

}