#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned u32;
typedef long s64;
typedef unsigned long u64;

#define internal static
#define global_var static


//rendering variables

global_var int screenWidth = 960;
global_var int screenHeight = 540;
global_var int screenOffset = 0;
global_var int yOffset = 0;
global_var int xOffset = 0;
global_var float render_scale = (float) screenHeight / 540;

struct RenderWindow {
	int height, width, sizeInBits;
	u8 bitS;
	void* memory;

	BITMAPINFO bitmap_info;
};

struct CropInfo {
	u16 cx;
	u16 cy;
	u16 cw;
	u16 ch;

	CropInfo() {}
	~CropInfo() {}
	CropInfo(u16 cx, u16 cy, u16 cw, u16 ch) : cx(cx), cy(cy), cw(cw), ch(ch) {}

	CropInfo& set(u16 x, u16 y, u16 w, u16 h) {
		cx = x, cy = y;
		cw = w, ch = h;
	}
};

global_var RenderWindow renderWindow;

global_var float delta_time;
#define dt delta_time


#define arrContainsBool(element, arr, sizeOfE) find(arr, arr + sizeof(arr) / sizeOfE, element) != arr + sizeof(arr) / sizeOfE
#define vecContainsBool(element, vec) find(vec.begin(), vec.end(), element) != vec.end()

#include "MathUtils.cpp"

#include "ImageUtils.cpp"

#include "RGBUtils.cpp"