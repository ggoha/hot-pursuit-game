#pragma once
#ifndef TREE_20100118
#define TREE_20100118
#include <Windows.h>
#include <vector>
#include <math.h>
#include <glut.h>
#include "CMap.h"
#include "SOIL.h"
#include "Car.h"
#include "CGame.h"
/* OpenGL needs static functions and command line arguments for drawing*/

class Drawing {
public:
	Drawing(Game* game, Map &map_data, std::vector<Car> &_cars );
	void draw( int argc, char * argv[] );
	static void Load_texture( const char*, GLuint& texture );

private:
	static void normalKeyHandler( unsigned char key, int x, int y );
	static void mouseButton(int button, int state, int x, int y);
	static int clickButton(int x, int y);
	static void display();

	static void drawMenu();
	static void renderBitmapString(float x, float y, float z, void *font, char *string);
	static void reshape( int width, int height );
	static void load();
	static void timer( int value );
	static Map map;
	static Game* game;
	static std::vector<Car> cars;
	static void OnMove( int direction, Size size );
	static bool menu;
};
#endif /* TREE_20100118 */