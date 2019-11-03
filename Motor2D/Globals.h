#pragma once
#include <windows.h>
#include <stdio.h>
#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

typedef unsigned int uint;

enum collider_category : uint16
{
	BALL = 0x0001,
	REGULAR_MAP = 0x0002,
	RAIL = 0x0003,
	RAIL_BALL = 0x0004,
	RAIL_ENTRANCE = 0x005,
	RAIL_BALL_ENTRANCE = 0x010
};

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum flipper_direction
{
	FL_NONE,

	FL_LEFT,
	FL_RIGHT
};

// Configuration -----------
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 456
#define SCREEN_SIZE 2
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE false
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "Sonic Pinball Party || CITM - Aaron Guerrero - Alex Melenchon"