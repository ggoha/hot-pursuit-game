#pragma once

#include <fstream>
#include <iostream>
#include "CPlayer.h"
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
	MapFileInput( const Field& newField, const Size& newSize, const std::vector<Coordinates>& positions, const Line& line ) :
		field( newField ), size( newSize ), startPositions( positions ), finishLine( line )
	{
	}

	Field field;
	Size size;
	std::vector<Coordinates> startPositions;
	Line finishLine;
};

class Reader {

public:
	Reader();

	~Reader();

	MapFileInput readFile( const std::string& path );

	PlayersInfo readPlayers();

	int readPlayersChoice( size_t num );
};