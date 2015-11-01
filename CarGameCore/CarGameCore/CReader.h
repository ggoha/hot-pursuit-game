#pragma once

#include <fstream>
#include <map>
#include <iostream>
#include "CPlayer.h"
#include "car.h"
#include <exception>
#include <string>
#include <assert.h>

typedef std::vector< std::vector<int> > Field;

struct Line {
	Line()
	{
		firstPoint = Coordinates( 0, 0 );
		secondPoint = Coordinates( 0, 0 );
	}

	Line( Coordinates x, Coordinates y ) :
		firstPoint( x ), secondPoint( y )
	{
	}

	Coordinates firstPoint;
	Coordinates secondPoint;
};

struct MapFileInput {
	MapFileInput()
	{
		size = Size( 0, 0 );
	}

	MapFileInput( const Field& newField, const Size& newSize, const std::vector<Coordinates>& positions, const std::vector<Car>& newCars, const Line& line ) :
		field( newField ), size( newSize ), startPositions( positions ), finishLine( line ), cars( newCars )
	{
	}

	Field field;
	Size size;
	std::vector<Coordinates> startPositions;
	std::vector<Car> cars;
	Line finishLine;
};

class Reader {
public:
	Reader();

	~Reader();

	MapFileInput readData( const std::string& mapPath, const std::string& carPath );

	PlayersInfo readPlayers();

	int readPlayersChoice( size_t num );

private:
	std::map< std::string, TColor > color_dict;

	std::pair<Field, std::vector<Coordinates>> readMap( const int n, const int m, std::ifstream& fin );

	Line readFinishLine( std::ifstream& fin );

	std::vector<Car> readCars( const std::string& carPath );
};