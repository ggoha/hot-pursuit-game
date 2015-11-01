#pragma once 
#include "coord.h"
#include "glut.h"
enum TColor { Red, Blue, Green };

#define PI 3.14159265

class Car
{
public:
	Car( std::vector<Coord> &coords_data, int _frames_per_step, TColor _color );
	Car( std::vector<Coord> &coords_data )
	{
		coords = coords_data;
		color = Red;
	}

	Car( TColor _color )
	{
		color = _color;
		frames_per_step = 200;
		current_step = 0;
		step_iteration = 0;
	}

	TColor Get_color()
	{
		return color;
	}

	void Push( Coord step )
	{
		coords.push_back( step );
	}

	void Calculate_angles();

	void Draw( float cell_size, WCoord indent );

	GLuint texture;
private:
	WCoord transate_to_wcoord( float x, float y, float cell_size, WCoord indent );
	void rotate( float &x, float &y, float angle );
	void rotate_car( float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &Dx, float &Dy, float centerX, float centerY, float angle );
	WCoord move( float cell_size, WCoord, float &angle );

	std::vector<Coord> coords;
	int current_step;
	int step_iteration;
	int frames_per_step;
	TColor color;
};