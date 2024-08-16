#include <windows.h>
#include "MyUtils.cpp"
#include "platformcommon.cpp"
#include "game.cpp";

#ifdef _MSC_VER
#    pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
	} break;

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);

		renderWindow.width = rect.right - rect.left;
		renderWindow.height = rect.bottom - rect.top;

		updateValues();

		renderWindow.sizeInBits = renderWindow.width * renderWindow.height * sizeof(u32);
		renderWindow.bitS = sizeof(u32);

		if (renderWindow.memory) VirtualFree(renderWindow.memory, 0, MEM_RELEASE);
		renderWindow.memory = VirtualAlloc(0, renderWindow.sizeInBits, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		renderWindow.bitmap_info.bmiHeader.biSize = sizeof(renderWindow.bitmap_info.bmiHeader);
		renderWindow.bitmap_info.bmiHeader.biWidth = renderWindow.width;
		renderWindow.bitmap_info.bmiHeader.biHeight = renderWindow.height;
		renderWindow.bitmap_info.bmiHeader.biPlanes = 1;
		renderWindow.bitmap_info.bmiHeader.biBitCount = 32;
		renderWindow.bitmap_info.bmiHeader.biCompression = BI_RGB;

	} break;

	default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
	return result;
}

internal void loop(HWND window) {
	//initilization variables for frames, input and render
	HDC hdc = GetDC(window);
	
	gameinit();

	delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	LARGE_INTEGER perf;
	QueryPerformanceFrequency(&perf);
	performance_frequency = (float)perf.QuadPart;

	//actual loop
	while (running) {
		// Input
		MSG message;
		
		setKeyUnchanged(&input);
		updateMouse(window, &mouse);

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

			//stupid WM_KEYDOWN doesnt detect mouse inputs
			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN: {
				processButtons((u32)message.wParam, &input, ((message.lParam & (1 << 31)) == 0));

			} break;

			case WM_SIZE: {
				clearEntireScreen(0x000000);
			} break;

			default: {
				TranslateMessage(&message);
				DispatchMessage(&message);
				
			}
			}

		}

		//i dont like this
		processMouseButtons(&input);

		// Simulate
		gameloop();

		// Render
		StretchDIBits(hdc, 0, 0, renderWindow.width, renderWindow.height, 0, 0, renderWindow.width, renderWindow.height, renderWindow.memory, &renderWindow.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// Frame calculator
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		
		while (delta_time < 0.01) {
			QueryPerformanceCounter(&frame_end_time);
			delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		}
		frame_begin_time = frame_end_time;

	}
}

int main() {
	
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	//ShowCursor(FALSE);

	// Create Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&window_class);

	// Create Window
	HWND window = CreateWindow(window_class.lpszClassName, "RobotSim", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

	//Fullscreen yolo

	SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
	SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	

	loop(window);
}