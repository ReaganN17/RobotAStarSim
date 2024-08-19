struct Button_State {
	bool down;
	bool changed;
};

enum zLayer:u8 {
	FARBACK, BACK, MIDDLE, FRONT, FARFRONT, UNRENDER
};

enum Inputs:u8 {
	ESC, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

	TINDLE, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, BACKSPACE,
	TAB, Q, W, E, R, T, Y, U, I, O, P, L_BRACKET, R_BRACKET, BACKSLASH,
	CAPS_LOCK, A, S, D, F, G, H, J, K, L, COLON, QUOTE, ENTER,
	L_SHIFT, Z, X, C, V, B, N, M, COMMA, PERIOD, FORSLASH, R_SHIFT,
	L_CTRL, L_ALT, SPACE, R_ALT, R_CTRL,

	SHIFT, CTRL, ALT,

	SNAPSHOT, SCROLL_LOCK, PAUSE,
	INSERT, HOME, PGUP, DEL, END, PGDOWN,

	PRINT, SELECT, EXECUTE, CLEAR, HELP, APPS, SLEEP,

	LEFT, UP, DOWN, RIGHT,

	NUM_LOCK, NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
	NUM_MULT, NUM_ADD, NUM_SEPARATOR, NUM_SUBTRACT, NUM_DECIMAL, NUM_DIVIDE,

	LEFT_MOUSE, RIGHT_MOUSE, MIDDLE_MOUSE, SIDE_FRONT_MOUSE, SIDE_BACK_MOUSE,

	BUTTON_COUNT,
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};

//i really hate how this is done but im too tired to make it look nicer
#define processButton(b, vk, d)\
case vk: {\
input -> buttons[b].changed = d != input -> buttons[b].down;\
input -> buttons[b].down = d;\
} break;

void processButtons(u32 vkcode, Input *input, bool d) {
	switch (vkcode) {
		processButton(LEFT, VK_LEFT, d);
		processButton(UP, VK_UP, d);
		processButton(DOWN, VK_DOWN, d);
		processButton(RIGHT, VK_RIGHT, d);

		processButton(ENTER, VK_RETURN, d);
		processButton(L_SHIFT, VK_LSHIFT, d);
		processButton(SHIFT, VK_SHIFT, d);
		processButton(CTRL, VK_CONTROL, d);

		processButton(ESC, VK_ESCAPE, d);

		processButton(W, 'W', d);
		processButton(S, 'S', d);
		processButton(A, 'A', d);
		processButton(D, 'D', d);
		processButton(Q, 'Q', d);
		processButton(E, 'E', d);
		processButton(Z, 'Z', d);
		processButton(X, 'X', d);

		processButton(KEY_0, '0', d);
		processButton(KEY_1, '1', d);
		processButton(KEY_2, '2', d);
		processButton(KEY_3, '3', d);
		processButton(KEY_4, '4', d);
	}
}


#define processMButton(b, vk)\
input->buttons[b].changed = (GetAsyncKeyState(vk) < 0) != input->buttons[b].down;\
input->buttons[b].down = (GetAsyncKeyState(vk) < 0);

void processMouseButtons(Input *input) {
	processMButton(LEFT_MOUSE, VK_LBUTTON);
}

void setKeyUnchanged(Input *input) {
	for (int i = 0; i < BUTTON_COUNT; i++) {
		input -> buttons[i].changed = false;
	}
}

void updateMouse(HWND window, POINT* mouse) {
	GetCursorPos(mouse);
	ScreenToClient(window, mouse);
	mouse->y = 270 - (mouse->y - yOffset) * (540.f / screenHeight);
	mouse->x = (mouse->x - (renderWindow.width * 0.5)) * (540.f / screenHeight);
	mouse->x = clamp(-480, mouse->x, 480);
	mouse->y = clamp(-270, mouse->y, 270);
}