
#pragma once 
#include "Common.h"
#include "glut.h"
enum TColor { Red, Green, Blue, Yellow };

#define PI 3.14159265

class Car {
public:
	Car( std::vector<Coord> &coords_data, int _frames_per_step, TColor _color, float inp_start_angle = PI );
	Car( std::vector<Coord> &coords_data , float inp_start_angle = PI) {
		coords = coords_data;
		color = Red;
		next_coords = Coord();
		current_coords = Coord();
		start_angle = inp_start_angle;
		current_angle = start_angle;
	}

	Car( TColor _color, float inp_start_angle = PI ) {
		color = _color;
		frames_per_step = 200;
		current_step = 0;
		step_iteration = 0;
		next_coords = Coord();
		current_coords = Coord();
		start_angle = inp_start_angle;
		current_angle = start_angle;
	}

	TColor Get_color() {
		return color;
	}

	void Push( Coord step ) {
		coords.push_back( step );
	}
	void MoveTo( Coord inp_next_coord, bool crash );

	void Calculate_angles();

	void Draw( float cell_size, WCoord indent );

	GLuint texture;
	int frames_per_step;
	Coord current_coords;
	Coord next_coords;
	float current_angle;
	float start_angle;
private:
	WCoord transate_to_wcoord( float x, float y, float cell_size, WCoord indent );
	void rotate( float &x, float &y, float angle );
	void rotate_car( float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &Dx, float &Dy, float centerX, float centerY, float angle );
	WCoord move( float cell_size, WCoord, float &angle );

	std::vector<Coord> coords;
	int current_step;
	int step_iteration;
	TColor color;

	
};