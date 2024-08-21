//rendering and gaming factors
//moved here so variables that are supposed to be interacted with are in one place

//mouse x and y position relative to coordinates
global_var POINT mouse;

//contains button inputs
global_var Input input = {};

#define isdown(b) input.buttons[b].down
#define pressed(b) (input.buttons[b].down && input.buttons[b].changed)
#define released(b) (!input.buttons[b].down && input.buttons[b].changed)

//runs the game, if set false will exit the window
global_var bool running = true;

//if backround doesnt exist, set to enable black bg;
global_var bool background = false;
//other bools
global_var bool render = true;
global_var bool update = true;
global_var bool runevents = true;
//^do not mess with names, meant to be constants

//basic elements
#include "TaffyGraphicEngine/renderer.cpp"
#include "TaffyGraphicEngine/events.cpp"
#include "TaffyGraphicEngine/objects.cpp"

static float debug_num = 0;

#include "Screen.cpp"

BasicEvent* screen = nullptr;

internal void gameinit() {
	clearEntireScreen(0x000000);
	screen = new MapScreen(&screen);
	screen->start();
}

internal void gameloop() {
	//default background
	if (background) renderBG();

	if (runevents) { runEvents(); }
	if (update) {updateAllObjects();}
	if (render) { renderAllObjects(); }

	draw_number(debug_num, 0, 0, 5);

	if (pressed(ESC)) { running = false; }
}