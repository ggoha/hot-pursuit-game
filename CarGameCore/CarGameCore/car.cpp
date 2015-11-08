
#include <vector>
#include "Car.h"

Car::Car( std::vector<Coord> &coords_data, int _frames_per_step, TColor _color = Red, float inp_start_angle )
	: coords( coords_data ), current_step( 0 ), step_iteration( 0 ),
	color( _color ), frames_per_step( _frames_per_step ) {
	next_coords = Coord();
	current_coords = Coord();
	start_angle = inp_start_angle;
	current_angle = start_angle;
}

void Car::rotate( float &x, float &y, float angle ) {
	float tx = x,
		ty = y;
	x = tx * cos( angle ) - ty * sin( angle );
	y = tx * sin( angle ) + ty * cos( angle );
}

void Car::rotate_car( float &Ax, float &Ay, float &Bx, float &By, float &Cx, float &Cy, float &Dx, float &Dy, float centerX, float centerY, float angle ) {
	Ax -= centerX;
	Bx -= centerX;
	Cx -= centerX;
	Dx -= centerX;

	Ay -= centerY;
	By -= centerY;
	Cy -= centerY;
	Dy -= centerY;

	rotate( Ax, Ay, angle );
	rotate( Bx, By, angle );
	rotate( Cx, Cy, angle );
	rotate( Dx, Dy, angle );

	Ax += centerX;
	Bx += centerX;
	Cx += centerX;
	Dx += centerX;

	Ay += centerY;
	By += centerY;
	Cy += centerY;
	Dy += centerY;
}


void Car::Draw( float cell_size, WCoord indent ) {
	step_iteration++;
	if( step_iteration >= frames_per_step ) {
		step_iteration = 0;
		current_coords = next_coords;
	}
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	float angle = 0.0;
	WCoord cord = move( cell_size, indent, angle );
	if( current_coords.x != next_coords.x || current_coords.y != next_coords.y ) {
		current_angle = angle;
	}
	float left = cord.x;
	float right = cord.x + cell_size;
	float bottom = cord.y + cell_size / 2;
	float top = cord.y;
	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBegin( GL_POLYGON );
	float Ax = left,
		Ay = bottom,
		Bx = right,
		By = bottom,
		Cx = right,
		Cy = top,
		Dx = left,
		Dy = top;

	float centerX = Dx - (Dx - Bx) / 2,
		centerY = Dy - (Dy - By) / 2;
	rotate_car( Ax, Ay, Bx, By, Cx, Cy, Dx, Dy, centerX, centerY, current_angle );
	glTexCoord2f( 0.0f, 0.0f ); glVertex3f( Ax, Ay, 0.0f );
	glTexCoord2f( 1.0f, 0.0f ); glVertex3f( Bx, By, 0.0f );
	glTexCoord2f( 1.0f, 1.0f ); glVertex3f( Cx, Cy, 0.0f );
	glTexCoord2f( 0.0f, 1.0f ); glVertex3f( Dx, Dy, 0.0f );
	glEnd();
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
	Sleep( 1 );
}

float find_angle( int x, int y ) {
	float angle = 0.0;
	int length = x * x + y * y;
	if( length != 0 ) {
		angle = (float)x / sqrt( length );
	}
	return acos( angle );
}

WCoord Car::move( float cell_size, WCoord indent, float &angle ) {
	float coord_x = next_coords.x - current_coords.x;
	float coord_y = next_coords.y - current_coords.y;
	float dx = (coord_x) / frames_per_step;
	float dy = (coord_y) / frames_per_step;
	float x = current_coords.x + step_iteration * dx;
	float y = current_coords.y + step_iteration * dy;
	float help_angle;
	next_coords.y - current_coords.y < 0?help_angle = PI:help_angle = 0;
	angle = abs( 2.0*help_angle - find_angle( coord_x, coord_y ) );
	return transate_to_wcoord( x, y, cell_size, indent );
}

void Car::MoveTo( Coord inp_next_coord, bool crash ) {
	next_coords = inp_next_coord;
	if( crash ) {
		current_coords = inp_next_coord;
		current_angle = start_angle;
	}
}



void Car::Calculate_angles() {
	for( int i = 1; i < coords.size(); ++i ) {
		coords[i - 1].angle = find_angle( coords[i].x - coords[i - 1].x, coords[i].y - coords[i - 1].y );
		if( coords[i].y - coords[i - 1].y < 0 ) {
			coords[i - 1].help_angle = PI;
		}
	}
	coords[coords.size() - 1].angle = coords[coords.size() - 2].angle;
	coords[coords.size() - 1].help_angle = coords[coords.size() - 2].help_angle;
}

// TODO добавить умножение на коэфициент сжатия чтобы соответствовать 2.5D
WCoord Car::transate_to_wcoord( float x, float y, float cell_size, WCoord indent ) {
	WCoord wcoord( 0, 0 );
	wcoord.x = x * cell_size + indent.x;
	wcoord.y = y * cell_size + indent.y;
	return wcoord;
}

