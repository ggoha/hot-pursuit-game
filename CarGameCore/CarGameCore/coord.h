#pragma once
// координаты в игре

struct Coord
{
	int x;
	int y;
	float angle;
	float help_angle = 0.0;
};

// координаты в окне
struct WCoord
{
	float x;
	float y;
	WCoord( float _x, float _y ) : x( _x ), y( _y ) {}
	WCoord() {}
};