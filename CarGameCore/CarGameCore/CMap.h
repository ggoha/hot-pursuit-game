#pragma once

#include <vector>
#include <iostream>

#include <glut.h>
#include "Common.h"
#include <memory>

typedef std::pair<size_t, size_t> Size;
typedef std::vector< std::vector<int> > Field; // was size_t here

class Map {
public:
	Map();

	Map( const Size& size, const Field& newMap );

	~Map();

	void print();
	void setPosition( int x, int y );
	void clearPosition( int x, int y );
	Size getSize();
	bool isEmpty( int i, int j );

	/*visual part*/
	Map( Field& map_data );
	void Calculate();
	void Draw();
	float Get_cell_size();
	WCoord Get_indent();
	Map getMapInOpenGLView();
	bool Need_to_reload();

	GLuint texture_board;
	GLuint texture_road;
	GLuint texture_map;

private:
	Field map; // Карта (таблица), в которой 1 - препятствие (дороги нет). А 0 - дорога.
	Size size;

	/*visual part*/
	void save_texture();
	void reload();
	float cell_size;
	WCoord indent;
	bool need_reload;
};