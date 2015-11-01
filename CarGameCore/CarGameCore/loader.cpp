#include "loader.h"
#include "CMap.h"
#include <vector>

Loader::Loader( std::string map_file, std::string cars_file, int argc, char* argv[] )
{
	color_dict["Red"] = Red;
	color_dict["Green"] = Green;
	color_dict["Blue"] = Blue;

	std::ifstream map_in( map_file );
	Map map = read_map( map_in );

	std::ifstream cars_in( cars_file );
	std::vector<Car> cars = read_cars( cars_in );

	Drawing drawing_module( map, cars );
	drawing_module.draw( argc, argv );
}

Map Loader::read_map( std::ifstream & input )
{
	int n = 0,
		m = 0,
		cell = 0;
	input >> n >> m;
	std::vector<std::vector<int>> map_data;
	map_data.resize( n );
	for( int i = n - 1; i >= 0; i-- ) {
		map_data[i].resize( m );
		for( int j = 0; j < m; j++ ) {
			input >> cell;
			map_data[i][j] = cell;
		}
	}
	Map map( map_data );
	return map;
}
std::vector<Car> Loader::read_cars( std::ifstream & input )
{
	int n = 0,
		number_of_steps = 0;
	std::string color = "";
	input >> n;
	std::vector<Car> cars;
	for( int i = 0; i < n; ++i ) {
		input >> color;
		input >> number_of_steps; // should it be the same for all cars?
		Car car( color_dict[color] );
		for( int j = 0; j < number_of_steps; ++j ) {
			Coord step;
			input >> step.x >> step.y;
			car.Push( step );
		}
		car.Calculate_angles();
		cars.push_back( car );
	}
	return cars;
}