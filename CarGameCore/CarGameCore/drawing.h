#pragma once
#include <Windows.h>
#include <vector>
#include <math.h>
#include <glut.h>
#include "CMap.h"
#include "SOIL.h"
#include "car.h"
/* OpenGL needs static functions and command line arguments for drawing*/

class Drawing
{
public:
	Drawing( Map &map_data, std::vector<Car> &_cars );
	Drawing() {};
	void draw( int argc, char * argv[] );
	static void Load_texture( const char*, GLuint& texture );

private:
	static void normalKeyHandler( unsigned char key, int x, int y );
	static void display();
	static void reshape( int width, int height );
	static void load();
	static void timer( int value );
	static Map map;
	static std::vector<Car> cars;
};
