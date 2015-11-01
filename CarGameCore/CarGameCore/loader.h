#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "drawing.h"
#include "car.h"
#include "coord.h"
#include "CMap.h"

const int DEFAULT_WINDOW_WIDTH = 500;
const int DEFAULT_WINDOW_HEIGHT = 500;
const int DEFAULT_STEPS_PER_CELL = 20;

class Loader
{
public:
	Loader( std::string map_file, std::string cars_file, int argc, char* argv[] ); // конструктор с указанным входным файлом
private:
	Map read_map( std::ifstream & input );
	std::vector<Car> read_cars( std::ifstream & input );

	Drawing drawing_module;
	std::map< std::string, TColor > color_dict;
};